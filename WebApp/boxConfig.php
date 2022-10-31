<?php

    $intTemp = 0;
    $intPressure = "";
    $intHumidity = "";

    $extTemp = 0;
    $extPressure = "";
    $extHumidity = "";
 
    $uppR = "";
    $uppG = "";
    $uppB = "";

    $lowR = "";
    $lowG = "";
    $lowB = "";

    $Ex_F_1 = 0;

    $topOnTime  = "";
    $topOffTime = "";
    $botOnTime  = "";
    $botOffTime = "";
    $topOn  = "";
    $botOn  = "";
    $topTimerOn = "";
    $botTimerOn = "";

    $cURL = curl_init();                                        //create curl connection
    
    curl_setopt($cURL, CURLOPT_URL, 'http://192.168.60.25');   //set options for curl
    curl_setopt($cURL, CURLOPT_RETURNTRANSFER, true);
    
    $contents = curl_exec($cURL);
    //$contents = "";
    
    curl_close($cURL);                                          //close curl connection

    if(strlen($contents)>10){

        $divided = explode('|', $contents);                         //break string based on pipe into an array
        
        foreach($divided as $passed)                                //loop through array
        {
            $settings = explode(':', $passed);                     //brek array string based on colon

            switch($settings[0]){                                   //check the name string to decide where to store the value
                case "intTemp":
                    $intTemp = $settings[1];
                break;
                case "intPressure":
                    $intPressure = $settings[1];
                break;
                case "intHumidity":
                    $intHumidity = $settings[1];
                break;

                case "extTemp":
                    $extTemp = $settings[1];
                break;
                case "extPressure":
                    $extPressure = $settings[1];
                break;
                case "extHumidity":
                    $extHumidity = $settings[1];
                break;

                case "uppR":
                    $uppR = $settings[1];
                break;
                case "uppG":
                    $uppG = $settings[1];
                break;
                case "uppB":
                    $uppB = $settings[1];
                break;
                
                case "lowR":
                    $lowR = $settings[1];
                break;
                case "lowG":
                    $lowG = $settings[1];
                break;
                case "lowB":
                    $lowB = $settings[1];
                break;

                case "Ex_F_1":
                    $Ex_F_1 = $settings[1];
                break;

                case "topOnTime":
                    $topOnTime = $settings[1];
                break;

                case "topOffTime":
                    $topOffTime = $settings[1];
                break;

                case "botOnTime":
                    $botOnTime = $settings[1];
                break;

                case "botOffTime":
                    $botOffTime = $settings[1];
                break;

                case "topOn":
                    $topOn = $settings[1];
                break;

                case "botOn":
                    $botOn = $settings[1];
                break;
 
                case "topTimerOn":
                    $topTimerOn = $settings[1];
                break;

                case "botTimerOn":
                    $botTimerOn = $settings[1];
                break;

                case "topTimeLeft":
                    $topTimeLeft = $settings[1];
                break;

                case "botTimeLeft":
                    $botTimeLeft = $settings[1];
                break;

            }                                   
        }

        #calculation for top timer display
        if ($topTimerOn)
        {
            $topTimeLeft = $topTimeLeft/60000;

            if ($topOn){
                $topTimeLeft = ($topOnTime) - $topTimeLeft;
            }else{
                $topTimeLeft = ($topOffTime) - $topTimeLeft;
            }

            $topTimeLeft = round($topTimeLeft, 2);
                
        }else{

            $topTimeLeft = 0;
        }
        
        #calculation for bot timer display
        if ($botTimerOn)
        {
            $botTimeLeft = $botTimeLeft/60000;

            if ($botOn){
                $botTimeLeft = ($botOnTime) - $botTimeLeft;
            }else{
                $botTimeLeft = ($botOffTime) - $botTimeLeft;
            }

            $botTimeLeft = round($botTimeLeft, 2);
                
        }else{

            $botTimeLeft = 0;
        }
    }
?>

<!doctype html>

<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Grow Box V1.0</title>

<meta name="description" content="Grow Box V1.0">
<meta name="author" content="TSL">

<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<link rel="stylesheet" type="text/css" href="boxStyle.css">
</head>

<body>
    <div class="mainWrapper">
        
        <div class="header">
            <p><h1>Grow Box V1.0</h1></p>
        </div>

        <div class="upperFlex">
            
            <div class="readData">
                <h2><u>Internal Data</u></h2>   
                <table>
                    <tr><td>Int Temperature</td><td>:</td><td><?=round($intTemp,2) ?> C</td><td><?=round((($intTemp * 9/5) + 32),2)?> F</td></tr>
                    <tr><td>Int Pressure</td><td>:</td><td><?=round($intPressure,2) ?></td></tr>
                    <tr><td>Int Humidity</td><td>:</td><td><?=round($intHumidity,2) ?></td></tr>
                </table>
            </div>
            
            <div class="readData">
                <h2><u>External Data</u></h2> 
                <table>
                    <tr><td>Ext Temperature</td><td>:</td><td><?=round($extTemp,2) ?>C</td><td><?=round((($extTemp * 9/5) + 32),2)?> F</td></tr>
                    <tr><td>Ext Pressure</td><td>:</td><td><?=round($extPressure,2) ?></td></tr>
                    <tr><td>Ext Humidity</td><td>:</td><td><?=round($extHumidity,2) ?></td></tr>
                </table>
            </div>
        
        </div>

        <div class="controlls">
            <form class="updateForm">
                <h2><u>Upper Box Control</u></h2>
                <div class="boxInfoFlex">
                    <div class="boxInfo">
                        <input type="color" class="colorPicker" id="upperBoxColor" value="#<?=(dechex($uppR) . dechex($uppG) . dechex($uppB))?>"></br>
                        <label>Light Color</label></br>
                    </div>
                    <div class="boxInfo">Time Remaining: <?= $topTimeLeft ?> min</div>
                    <div class="boxInfo">
                        <label>Enter "On" Timer Length (hours)</label></br>
                        <input type="number" step=".001" id="topOnTime" value="<?= $topOnTime/60 ?>"></br>
                        <label>Enter "Off" Timer Length (hours)</label></br>
                        <input type="number" step=".001" id="topOffTime" value="<?= $topOffTime/60 ?>"></br>
                        <label>Timer Started?</label> 
                        <input type="checkbox" id="topOn"></br>
                        <label>Timer - Lights Active?</label> 
                        <input type="checkbox" id="topTimerOn"></br>
                    </div>
                </div>
                
                <h2><u>Lower Box Control</u></h2>
                <div class="boxInfoFlex">
                    
                    <div class="boxInfo">
                        <input type="color" class="colorPicker" id="lowerBoxColor" value="#<?=(dechex($lowR) . dechex($lowG) . dechex($lowB))?>"></br>
                        <label>Light Color</label></br>
                    </div>
                    <div class="boxInfo">Time Remaining: <?= $botTimeLeft ?> min</div>
                    <div class="boxInfo">
                        <label>Enter "On" Timer Length (hours)</label></br>
                        <input type="number" step=".001" id="botOnTime" value="<?= $botOnTime/60 ?>"></br>
                        <label>Enter "Off" Timer Length (hours)</label></br>
                        <input type="number" step=".001" id="botOffTime" value="<?= $botOffTime/60 ?>"></br>
                        <label>Timer Started?</label> 
                        <input type="checkbox" id="botOn"></br>
                        <label>Timer - Lights Active?</label>
                        <input type="checkbox" id="botTimerOn"></br>
                    </div>
                </div>

                <div id="exhaustControlWrapper">
                    <h2><u>Exhaust Fan Speed</u></h2>
                    <div class="slidecontainer">
                        <input type="range" list="tickmarks" value="<?=$Ex_F_1?>" id="exhaust">

                        <datalist id="tickmarks">
                            <option value="0"></option>
                            <option value="10"></option>
                            <option value="20"></option>
                            <option value="30"></option>
                            <option value="40"></option>
                            <option value="50"></option>
                            <option value="60"></option>
                            <option value="70"></option>
                            <option value="80"></option>
                            <option value="90"></option>
                            <option value="100"></option>
                        </datalist>
                        <label id="exhaustFanLabel"><?=$Ex_F_1?>%</label></br>
                    </div>
                </div>
                <button onclick="updateTimers()">Update</button>
            </form>
        </div>
    </div>
</body>
<script>
    $( document ).ready(function() {
        $("#upperBoxColor").change(function(){
            hexUpperString = $(this).val();
            //updateBox();
            console.log($(this).val());
        });
        
        $("#lowerBoxColor").change(function(){
            hexLowerString = $(this).val();
            //updateBox();
            console.log($(this).val());
        });

        $("#exhaust").change(function(){
            exhaust = $(this).val();
            //updateBox();
            console.log($(this).val()); 
        });
        
        $("#topOn").prop('checked', <?php echo $topOn ?>);
        $("#botOn").prop('checked', <?php echo $botOn ?>);
        $("#topTimerOn").prop('checked', <?php echo $topTimerOn ?>);
        $("#botTimerOn").prop('checked', <?php echo $botTimerOn ?>);

    });

    function updateTimers()
    {
        topOnTime = $("#topOnTime").val();
        topOffTime = $("#topOffTime").val();
        botOnTime = $("#botOnTime").val();
        botOffTime = $("#botOffTime").val();
        
        $("#topOn").is(":checked") ? topOn = "1" : topOn = 0;
        $("#botOn").is(":checked") ? botOn = "1" : botOn = 0;
        $("#topTimerOn").is(":checked") ? topTimerOn = "1" : topTimerOn = 0;
        $("#botTimerOn").is(":checked") ? botTimerOn = "1" : botTimerOn = 0;

        try{
            boxResponse = $.ajax({
                type: 'POST',
                url: '/postChangeTimers.php',
                data: { topOnTime: topOnTime,
                        topOffTime: topOffTime,
                        botOnTime: botOnTime,
                        botOffTime: botOffTime,
                        topOn: topOn,
                        botOn: botOn,
                        topTimerOn: topTimerOn,
                        botTimerOn: botTimerOn
                }
            });
            
            console.log(boxResponse);  //log our error

        } catch(error)
        {
            console.error(error);
        }

        updateBox();
    }

    function updateBox()
    {
        hexUpperString = $("#upperBoxColor").val();
        hexLowerString = $("#lowerBoxColor").val();

        exhaust = $("#exhaust").val();
        $("#exhaustFanLabel").text(exhaust + "%");

        rConvUp = hexUpperString[1] + hexUpperString[2];  //concatonate left most 2 hex values
        gConvUp = hexUpperString[3] + hexUpperString[4];
        bConvUp = hexUpperString[5] + hexUpperString[6];
        
        rConvLo = hexLowerString[1] + hexLowerString[2];  //concatonate left most 2 hex values
        gConvLo = hexLowerString[3] + hexLowerString[4];
        bConvLo = hexLowerString[5] + hexLowerString[6];

        rUpper = parseInt(rConvUp, 16);                   //converts rConv to decimal
        gUpper = parseInt(gConvUp, 16);                   //converts rConv to decimal
        bUpper = parseInt(bConvUp, 16);                   //converts rConv to decimal

        rLower = parseInt(rConvLo, 16);                   //converts rConv to decimal
        gLower = parseInt(gConvLo, 16);                   //converts rConv to decimal
        bLower = parseInt(bConvLo, 16);                   //converts rConv to decimal

        try{
            boxResponse = $.ajax({
                type: 'POST',
                url: '/postChange.php',
                data: { R_1: rUpper,
                        G_1: gUpper,
                        B_1: bUpper,
                        R_2: rLower,
                        G_2: gLower,
                        B_2: bLower,
                        Ex_F_1: exhaust
                }
            });
            
            console.log(boxResponse);  //log our error

        } catch(error)
        {
            console.error(error);
        }   
    }

</script>
</html>