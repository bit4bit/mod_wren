class FreeswitchAPI {
      construct new() {
      }

      foreign execute(api, arg)
}

class Freeswitch {
      foreign static consoleLog(level, msg)
      static API() {
        return FreeswitchAPI.new()
      }
}
