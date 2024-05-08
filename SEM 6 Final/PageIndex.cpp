const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
  <head>
    <title>ESP32 ESP-NOW & WEB SERVER (CONTROLLING)</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      p {font-size: 1.2rem;}
      body {margin: 0;}
      .topnav {overflow: hidden; background-color: #6C0BA9; color: white; font-size: 1.5rem;}
      .content {padding: 20px; }
      .card {background-color: white; box-shadow: 0px 0px 10px 1px rgba(140,140,140,.5); border: 1px solid #6C0BA9; border-radius: 15px;}
      .card.header {background-color: #6C0BA9; color: white; border-bottom-right-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 12px; border-top-left-radius: 12px;}
      .cards {max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));}
      .reading {font-size: 2.8rem;}
      .packet {color: #bebebe;}
      .temperatureColor {color: #fd7e14;}
      .humidityColor {color: #1b78e2;}
      .LEDColor {color: #183153;}
      
      /* ----------------------------------- Toggle Switch */
      .switch {
        position: relative;
        display: inline-block;
        width: 70px;
        height: 34px;
      }

      .switch input {display:none;}

      .sliderTS {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #D3D3D3;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 34px;
      }

      .sliderTS:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: #f7f7f7;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 50%;
      }

      input:checked + .sliderTS {
        background-color: #62c934;
      }

      input:focus + .sliderTS {
        box-shadow: 0 0 1px #2196F3;
      }

      input:checked + .sliderTS:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(36px);
      }

      .sliderTS:after {
        content:'OFF';
        color: white;
        display: block;
        position: absolute;
        transform: translate(-50%,-50%);
        top: 50%;
        left: 70%;
        font-size: 10px;
        font-family: Verdana, sans-serif;
      }

      input:checked + .sliderTS:after {  
        left: 25%;
        content:'ON';
      }
      /* ----------------------------------- */
      
      /* ----------------------------------- Slider */
      .slidecontainer {
        width: 100%;
      }

      .slider {
        -webkit-appearance: none;
        width: 50%;
        height: 10px;
        border-radius: 5px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }

      .slider:hover {
        opacity: 1;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #1b78e2;
        cursor: pointer;
      }

      .slider::-moz-range-thumb {
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #1b78e2;
        cursor: pointer;
      }
      /* ----------------------------------- */
    </style>
  </head>
  
  <body>
    <div class="topnav">
      <h3> Vibraile </h3>
    </div>
    
    <br>

    <div class="content">
        <form id="commandForm" onsubmit="findcode(event)">
          <input type="text" id="commandInput" placeholder="Enter letter">
          <button type="submit">Submit</button>
        </form>
        
        
    </div>

    
    <div class="content">
      <div class="cards">
         
        <div class="card">
          <div class="card header">
            <h2>Left Hand side</h2>
          </div>
          <br>
          <h4 class="LEDColor">sensor 1: </h4> 
          <label class="switch">
            <input type="checkbox" id="togLED1" onclick="send_LED_State_Cmd('ESP32Slave1','togLED1','5')">
            <div class="sliderTS"></div>
          </label>
          <br>
		  <br>
          <h4 class="LEDColor">sensor 2: </h4> 
          <label class="switch">
            <input type="checkbox" id="togLED2" onclick="send_LED_State_Cmd('ESP32Slave1','togLED2','18')">
            <div class="sliderTS"></div>
          </label>
          <br>
		  <br>
          <h4 class="LEDColor">sensor 3: </h4> 
          <label class="switch">
            <input type="checkbox" id="togLED3" onclick="send_LED_State_Cmd('ESP32Slave1','togLED3','19')">
            <div class="sliderTS"></div>
          </label>
          <br>
        </div>
        
        <div class="card">
          <div class="card header">
            <h2>Right Hand Side</h2>
          </div>
          <br>
          <h4 class="LEDColor">sensor 4: </h4> 
          <label class="switch">
            <input type="checkbox" id="togLED4" onclick="send_LED_State_Cmd('ESP32Slave2','togLED4','5')">
            <div class="sliderTS"></div>
          </label>
          <br>
		  <br>
		  <h4 class="LEDColor">sensor 5: </h4>
		  <label class="switch">
            <input type="checkbox" id="togLED5" onclick="send_LED_State_Cmd('ESP32Slave2','togLED5','18')">
            <div class="sliderTS"></div>
          </label>
          <br>
		  <br>
		  <h4 class="LEDColor">sensor 6: </h4>
		  <label class="switch">
            <input type="checkbox" id="togLED6" onclick="send_LED_State_Cmd('ESP32Slave2','togLED6','19')">
            <div class="sliderTS"></div>
          </label>
          <br>
		  <br>
        </div>
        
      </div>
    </div>
    
    <script>
      const brailleAlphabet = [
  {"A": [1, 0, 0, 0, 0, 0]},
  {"B": [1, 1, 0, 0, 0, 0]},
  {"C": [1, 0, 0, 1, 0, 0]},
  {"D": [1, 0, 0, 1, 1, 0]},
  {"E": [1, 0, 0, 0, 1, 0]},
  {"F": [1, 1, 0, 1, 0, 0]},
  {"G": [1, 1, 0, 1, 1, 0]},
  {"H": [1, 1, 0, 0, 1, 0]},
  {"I": [0, 1, 0, 1, 0, 0]},
  {"J": [0, 1, 0, 1, 1, 0]},
  {"K": [1, 0, 1, 0, 0, 0]},
  {"L": [1, 1, 1, 0, 0, 0]},
  {"M": [1, 0, 1, 1, 0, 0]},
  {"N": [1, 0, 1, 1, 1, 0]},
  {"O": [1, 0, 1, 0, 1, 0]},
  {"P": [1, 1, 1, 1, 0, 0]},
  {"Q": [1, 1, 1, 1, 1, 0]},
  {"R": [1, 1, 1, 0, 1, 0]},
  {"S": [0, 1, 1, 1, 0, 0]},
  {"T": [0, 1, 1, 1, 1, 0]},
  {"U": [1, 0, 1, 0, 0, 1]},
  {"V": [1, 1, 1, 0, 0, 1]},
  {"W": [0, 1, 0, 1, 1, 1]},
  {"X": [1, 0, 1, 1, 0, 1]},
  {"Y": [1, 0, 1, 1, 1, 1]},
  {"Z": [1, 0, 1, 0, 1, 1]}
];

      //------------------------------------------------------------ The variables for the slider.
      var sliderLED1 = document.getElementById("mySlider1");
      var last_sliderLED1_val = 0;
      var sliderLED2 = document.getElementById("mySlider2");
      var last_sliderLED2_val = 0;
      let sliders_used = "";
      //------------------------------------------------------------
      
      //------------------------------------------------------------ The variables for the Timer.
      var myTmr_send;
      var myTmr_send_interval = 250;
      var myTmr_send_start = 1;
      var myTmr_count_to_stop = 0;
      //------------------------------------------------------------
      
      //------------------------------------------------------------ The function called by "Toggle Switch" to control the LED.
      function send_LED_State_Cmd(board,id,gpio) {
        var tgLEDFlash = document.getElementById(id);
        var tgState;

        if (tgLEDFlash.checked == true) tgState = 1;
        if (tgLEDFlash.checked == false) tgState = 0;

        send_cmd(board,gpio,tgState);
      }
      //------------------------------------------------------------
      function findcode(event) {
            event.preventDefault(); // Prevent the default form submission behavior
            var input = document.getElementById('commandInput');
            var word = input.value;
            

            console.log(word);
            

            var size = word.length;

            loopWithDelay(0);


            function loopWithDelay(index) {
                if (index < size) {
                    setTimeout(function() {
                        var ch =word[index];
                        console.log(ch);
                        var arr = findCharacterInArray(ch);
                        console.log(arr);

                        for(var i=1;i<=6;i++){
                            var bi = arr[i-1];
                            if(bi==1){
                                var LED = document.getElementById(`togLED${i}`);

                                LED.checked = !LED.checked;
                                LED.dispatchEvent(new Event("click"));


                            }
                        }

                    

                    // Set a timeout to execute the function after 3 seconds (3000 milliseconds)
                        setTimeout(delayedFunction, 2000);

                        function delayedFunction() {
                            for(var i=1;i<=6;i++){
                                var bi = arr[i-1];
                                if(bi==1){
                                    var LED = document.getElementById(`togLED${i}`);
                                    LED.checked = !LED.checked;
                                    LED.dispatchEvent(new Event("click"));


                                }
                            }
                            
                        }
                        
                        loopWithDelay(index + 1); // Call the function recursively with the next index
                    }, 4000); // 1000 milliseconds (1 second) delay
                }
            }

            input.value="";




        }


        

        function findCharacterInArray(ch) {
            for (var i = 0; i < brailleAlphabet.length; i++) {
                var obj = brailleAlphabet[i];
                for (var key in obj) {
                    if (key === ch.toUpperCase()) {
                        return obj[key];
                    }
                }
            }
            return null; // Return null if character is not found
        }

      //------------------------------------------------------------ Function to detect when the slider is used and get its value and activate the Timer.
    //   sliderLED1.oninput = function() {
    //     sliders_used = "SL1";
    //     myTmr_count_to_stop = 0;
    //     if (myTmr_send_start == 1) {
    //       myTmr_send = setInterval(myTmr_send_LED_PWM_Cmd, myTmr_send_interval);
    //       myTmr_send_start = 0;
    //     }
    //   }
      
    //   sliderLED2.oninput = function() {
    //     sliders_used = "SL2";
    //     myTmr_count_to_stop = 0;
    //     if (myTmr_send_start == 1) {
    //       myTmr_send = setInterval(myTmr_send_LED_PWM_Cmd, myTmr_send_interval);
    //       myTmr_send_start = 0;
    //     }
    //   }
      // ------------------------------------------------------------
      
      // ---------------------------------------------------------------------- Timer for sending slider values and other data from this page to ESP32.
      function myTmr_send_LED_PWM_Cmd() {
        if (sliders_used == "SL1") {
          if (last_sliderLED1_val != sliderLED1.value) {
            send_cmd('ESP32Slave1',12,sliderLED1.value);
          }
          last_sliderLED1_val = sliderLED1.value;
        }
        
        if (sliders_used == "SL2") {
          if (last_sliderLED2_val != sliderLED2.value) {
            send_cmd('ESP32Slave2',12,sliderLED2.value);
          }
          last_sliderLED2_val = sliderLED2.value;
        }
        
        myTmr_count_to_stop++;
        if (myTmr_count_to_stop > 5) {
          myTmr_send_start = 1;
          clearInterval(myTmr_send);
        }
      }
      // ---------------------------------------------------------------------- 
      
      // ---------------------------------------------------------------------- XMLHttpRequest to submit data.
      function send_cmd(board,gpio,value) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "set_LED?board="+board+"&gpio_output="+gpio+"&val="+value, true);
        xhr.send();
      }
      // ---------------------------------------------------------------------- 
    </script>
  </body>
</html>
)=====";
