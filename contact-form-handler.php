<?php
$name= $_POST['name'];//grab name that was inputted in
$visitor_email = $_POST['email'];//grab email that was inputted in
$serv = $_POST['serv'];//grab service inputted in
$message=$_POST['subject'];//grab subject inputted in

$email_from='sebascrespo1995@gmail.com';//email that it is being sent from
$email_subject = "New Form Submission";//subject of the new email

$email_body = "User Name: $name.\n".//every thing that is going to be on the email
                "User Email: $visitor_email.\n".
                "User Service: $serv.\n".
                "Username Subject: $message.\n";
$to="sebas071995@hotmail.com";//where the email is going
$headers= "From: $email_from\r\n";//headers for the email
$headers .= "Reply-To: $visitor_email \r\n";
mail($to,$email_subject,$email_body, $headers );

header("Location:index.html");//what is being sent from

?>