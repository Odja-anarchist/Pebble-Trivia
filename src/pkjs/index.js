var targetURL = "https://opentdb.com/api.php?amount=1";

function xhrRequest(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}

function loadQuestion() {
  xhrRequest(targetURL, 'GET', function(responseText) {
     var json = JSON.parse(responseText);
    console.log('RESP: '+json);
  });
}

Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    loadQuestion();
  }
);
