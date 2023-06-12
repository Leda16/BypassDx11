<?php
session_start();
include('conexao.php');

if(empty($_POST['usuario'])) {
	header('Location: ');
	exit();
}

$usuario = mysqli_real_escape_string($conexao, $_POST['usuario']);

$query = "SELECT * FROM `license_keys` WHERE license_key = '{$usuario}'";


$result = mysqli_query($conexao, $query);

$row = mysqli_num_rows($result);

if($row == 1) {
	$_SESSION['usuario'] = $usuario;
	header('Location: ./register.php');
	exit();
} else {
	$_SESSION['noauthenticado'] = true;
	header('Location: ./');
	exit();
}