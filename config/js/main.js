(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    console.log('Submit');

    var return_to = getQueryParam('return_to', 'pebblejs://close#');
		// Change the URL to the close URL that will pass config values back to phone
    document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
  });
}

function loadOptions() {
  console.log('hello');
  var $randomColorToggle = $('#randomColor');
  var $chosenColorSelect = $('#chosenColor');


  if (localStorage.randomColor) {
    $randomColorToggle[0].checked = localStorage.randomColor === 'true';
    $chosenColorSelect[0].value = localStorage.chosenColor;
  }
}

function getAndStoreConfigData() {
  var $randomColorToggle = $('#randomColor');
  var $chosenColorSelect = $('#chosenColor');

	// If Random color is selected, then overwrite chosenColor with -1
	// Then pass back to phone

  var options = {
    chosenColor: $randomColorToggle[0].checked
  };

  localStorage.randomColor = options.randomColor;
  localStorage.chosenColor = options.chosenColor;

  console.log('Got options: ' + JSON.stringify(options));
  return options;
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}
