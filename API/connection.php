<?php
$host = "localhost";
$dbname = "bypass";
$username = "root";
$password = "";
try{
    $db = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
}catch(Exception $e){
    die("Fatal error: ".$e->getMessage());
}
?>
