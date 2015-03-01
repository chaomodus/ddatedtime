function sendTimezoneToWatch() {
  // Get the number of seconds to add to convert localtime to utc
  var offsetMinutes = new Date().getTimezoneOffset() * 60;
  // Send it to the watch
  Pebble.sendAppMessage({ timezoneOffset: offsetMinutes })
}
