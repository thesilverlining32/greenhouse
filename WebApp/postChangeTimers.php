<?php

    $url = 'http://192.168.60.25/input';
    $myvars = 'topOnTime=' . $_POST['topOnTime'] 
            . '&topOffTime=' . $_POST['topOffTime']
            . '&botOnTime=' . $_POST['botOnTime']
            . '&botOffTime=' . $_POST['botOffTime']
            . '&topOn=' . $_POST['topOn']
            . '&botOn=' . $_POST['botOn']
            . '&topTimerOn=' . $_POST['topTimerOn']
            . '&botTimerOn=' . $_POST['botTimerOn'];

    $ch = curl_init( $url );
    curl_setopt( $ch, CURLOPT_POST, 1);
    curl_setopt( $ch, CURLOPT_POSTFIELDS, $myvars);
    curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt( $ch, CURLOPT_HEADER, 0);
    curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1);

    $response = curl_exec( $ch );
    echo $response;
?>