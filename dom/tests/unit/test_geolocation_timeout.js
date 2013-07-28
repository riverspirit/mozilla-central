const Cc = Components.classes;
const Ci = Components.interfaces;
const Cu = Components.utils;
const Cr = Components.results;

Cu.import("resource://testing-common/httpd.js");

var httpserver = null;
var geolocation = null;
var success = false;
var watchId = -1;

function geoHandler(metadata, response)
{
    var georesponse = {
        status: "OK",
        location: {
            lat: 42,
            lng: 42,
        },
        accuracy: 42,
    };
  var position = JSON.stringify(georesponse);
  response.processAsync();
  response.setStatusLine("1.0", 200, "OK");
  response.setHeader("Cache-Control", "no-cache", false);
  response.setHeader("Content-Type", "aplication/x-javascript", false);
  do_timeout(5000, function() {
    response.write(position);
    response.finish();
  });
}

function successCallback() {
  do_check_true(false);
  do_test_finished();
}

function errorCallback() {
  do_check_true(true);
  do_test_finished();
}

function run_test()
{
  do_test_pending();

  if (Cc["@mozilla.org/xre/app-info;1"].getService(Ci.nsIXULRuntime)
        .processType == Ci.nsIXULRuntime.PROCESS_TYPE_DEFAULT) {
    httpserver = new HttpServer();
    httpserver.registerPathHandler("/geo", geoHandler);
    httpserver.start(-1);
    var prefs = Cc["@mozilla.org/preferences-service;1"].getService(Ci.nsIPrefBranch);
    prefs.setBoolPref("geo.wifi.scan", false);
    prefs.setCharPref("geo.wifi.uri", "http://localhost:" +
                      httpserver.identity.primaryPort + "/geo");
    prefs.setBoolPref("geo.testing.ignore_ipc_principal", true);
  }

  geolocation = Cc["@mozilla.org/geolocation;1"].getService(Ci.nsISupports);
  geolocation.getCurrentPosition(successCallback, errorCallback, {timeout: 2000});
}
