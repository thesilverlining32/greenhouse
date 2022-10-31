<?php

    $url = 'http://192.168.60.25/input';
    $myvars =  'R_1=' . $_POST['R_1'] 
            . '&G_1=' . $_POST['G_1']
            . '&B_1=' . $_POST['B_1']
            . '&R_2=' . $_POST['R_2']
            . '&G_2=' . $_POST['G_2']
            . '&B_2=' . $_POST['B_2']
            . '&Ex_F_1=' . $_POST['Ex_F_1'];

    $ch = curl_init( $url );
    curl_setopt( $ch, CURLOPT_POST, 1);
    curl_setopt( $ch, CURLOPT_POSTFIELDS, $myvars);
    curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt( $ch, CURLOPT_HEADER, 0);
    curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1);

    $response = curl_exec( $ch );
    echo $response;
?>