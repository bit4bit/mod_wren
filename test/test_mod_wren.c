#include <test/switch_test.h>

FST_CORE_BEGIN("conf")
{
  FST_MODULE_BEGIN(mod_wren, mod_wren_test)
    {
      FST_SETUP_BEGIN()
        {
          fst_requires_module("mod_wren");
        }
      FST_SETUP_END();
      
      FST_TEST_BEGIN(run_script_test)
        {
          switch_stream_handle_t stream = { 0 };

          SWITCH_STANDARD_STREAM(stream);

          switch_api_execute("wrenrun", "test.wren", NULL, &stream);
          
          if (stream.data) {
            switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "WREN DATA: %s\n", (char *)stream.data);
            fst_check(strstr(stream.data, "+OK") == stream.data);
            free(stream.data);
          }
        }
      FST_TEST_END();

      FST_TEARDOWN_BEGIN()
        {
        }
      FST_TEARDOWN_END();
    }
  FST_MODULE_END();
}
FST_CORE_END();
