<?php
session_start();
include('conexao.php');

if(empty($_POST['usuario'])) {
	header('Location: ');
	exit();
}

$passwd = mysqli_real_escape_string($conexao, $_POST['passwd']);
$usuario = mysqli_real_escape_string($conexao, $_POST['usuario']);

$query = "SELECT * FROM `apipremium` WHERE username = '{$usuario}' AND password = '{$passwd}'";


$result = mysqli_query($conexao, $query);

$row = mysqli_num_rows($result);

if($row == 1) {
	$_SESSION['usuario'] = $usuario;
	header('Location: dashboard');
	exit();
} else {
	$_SESSION['noauthenticado'] = true;
	header('Location: ./');
	exit();
}