    var socket;
    var firstconnect = true,
        i2cNum  = "0x70",
	disp = [];
dispRed = [];
dispGreen = [];


// Create a matrix of LEDs inside the <table> tags.
var matrixData;
for(var j=7; j>=0; j--) {
	matrixData += '<tr>';
	for(var i=0; i<8; i++) {
	    matrixData += '<td><div class="LED" id="id'+i+'_'+j+
		'" onclick="LEDclick('+i+','+j+')">'+
		i+','+j+'</div></td>';
	    }
	matrixData += '</tr>';
}
$('#matrixLED').append(matrixData);

// Send one column when LED is clicked.
//color =0

function LEDclick(i, j) {
if (((dispRed[i] >> j) & 0x1) === 0 && ((dispGreen[i] >> j) & 0x1) === 0){ // off
$('#id'+i+'_'+j).addClass('red')
dispGreen[i] ^= 0x0<<j;
dispRed[i] ^= 0x1<<j;


} else if (((dispRed[i] >> j) & 0x1) === 1 && ((dispGreen[i] >> j) & 0x1) === 0){ // red
$('#id'+i+'_'+j).removeClass('red')
$('#id'+i+'_'+j).addClass('green')
dispGreen[i] ^= 0x1<<j;
dispRed[i] ^= 0x1<<j;

} else if (((dispRed[i] >> j) & 0x1) === 0 && ((dispGreen[i] >> j) & 0x1) === 1){ // green

$('#id'+i+'_'+j).removeClass('green')
$('#id'+i+'_'+j).addClass('orange')
dispGreen[i] ^= 0x0<<j;
dispRed[i] ^= 0x1<<j;

} else if (((dispRed[i] >> j) & 0x1) === 1 && ((dispGreen[i] >> j) & 0x1) === 1){ // orange
$('#id'+i+'_'+j).removeClass('orange')
dispGreen[i] ^= 0x1<<j;
dispRed[i] ^= 0x1<<j;


}

console.log('0x'+dispGreen[i].toString(16));

    socket.emit('i2cset', {i2cNum: i2cNum, i: 2*i, 
			     disp: '0x'+dispGreen[i].toString(16)});
socket.emit('i2cset', {i2cNum: i2cNum, i: 2*i+1, 
			     disp: '0x'+dispRed[i].toString(16)});


}

    function connect() {
      if(firstconnect) {
        socket = io.connect(null);

        // See https://github.com/LearnBoost/socket.io/wiki/Exposed-events
        // for Exposed events
        socket.on('message', function(data)
            { status_update("Received: message " + data);});
        socket.on('connect', function()
            { status_update("Connected to Server"); });
        socket.on('disconnect', function()
            { status_update("Disconnected from Server"); });
        socket.on('reconnect', function()
            { status_update("Reconnected to Server"); });
        socket.on('reconnecting', function( nextRetry )
            { status_update("Reconnecting in " + nextRetry/1000 + " s"); });
        socket.on('reconnect_failed', function()
            { message("Reconnect Failed"); });

        socket.on('matrix',  matrix);
        // Read display for initial image.  Store in disp[]
        socket.emit("matrix", i2cNum);

        firstconnect = false;
      }
      else {
        socket.socket.reconnect();
      }
    }

    function disconnect() {
      socket.disconnect();
    }

    // When new data arrives, convert it and display it.
    // data is a string of 16 values, each a pair of hex digits.
    function matrix(data) {
        var i, j;
        disp = [];
        //        status_update("i2c: " + data);
        // Make data an array, each entry is a pair of digits
        data = data.split(" ");
for (i = 0; i < 8; i++) {
for (j = 0; j < 8; j++) {
$('#id' + i + '_' + j).removeClass('green');
$('#id' + i + '_' + j).removeClass('red');
$('#id' + i + '_' + j).removeClass('orange');
}
}
        //        status_update("data: " + data);
        // Every other pair of digits are Green. The others are red.
        // Ignore the red.
        // Convert from hex.
        for (i = 0; i < data.length; i += 2) {
            dispGreen[i / 2] = parseInt(data[i], 16);
        }
        //        status_update("disp: " + disp);
        // i cycles through each column
        for (i = 0; i < dispGreen.length; i++) {
            // j cycles through each bit
            for (j = 0; j < 8; j++) {
                if (((dispGreen[i] >> j) & 0x1) === 1) {
                    $('#id' + i + '_' + j).addClass('green');
                } else {
                    $('#id' + i + '_' + j).removeClass('green');
                }
            }
        }

for (i = 1; i < data.length; i += 2) {
            dispRed[((i+1) / 2)-1] = parseInt(data[i], 16);

        }

        //        status_update("disp: " + disp);
        // i cycles through each column
        for (i = 0; i < dispRed.length; i++) {


            // j cycles through each bit
            for (j = 0; j < 8; j++) {

                if (((dispRed[i] >> j) & 0x1) === 1) {
		if ($('#id' + i + '_' + j).hasClass('green')) {
$('#id' + i + '_' + j).addClass('orange');
} else {
                    $('#id' + i + '_' + j).addClass('red');
}
                } else {
                    $('#id' + i + '_' + j).removeClass('red');
                }
            }
        }
console.log(dispRed);
console.log(dispGreen);

}


    function status_update(txt){
	$('#status').html(txt);
    }

    function updateFromLED(){
      socket.emit("matrix", i2cNum);    
    }

connect();

$(function () {
    // setup control widget
    $("#i2cNum").val(i2cNum).change(function () {
        i2cNum = $(this).val();
    });
});
