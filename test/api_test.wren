import "freeswitch" for Freeswitch

var api = Freeswitch.API()
var result = api.execute("url_encode", "echo")
System.print(result)