<?php
session_start();
include('conexao.php');

if (empty($_POST['usuario']) || empty($_POST['passwd'])) {
    header('Location: ./ ');
    exit();
}

$usuario = mysqli_real_escape_string($conexao, $_POST['usuario']);
$senha = $_POST['passwd'];

$query = "SELECT password FROM `usuarios` WHERE username = '{$usuario}'";
$result = mysqli_query($conexao, $query);
$row = mysqli_fetch_assoc($result);

if ($row) {
    $hashArmazenadoNoBanco = $row['password'];
    
    if (password_verify($senha, $hashArmazenadoNoBanco)) {
        $_SESSION['usuario'] = $usuario;
        header('Location: dashboard');
        exit();
    } else {
        $_SESSION['noauthenticado'] = true;
        header('Location: ./');
        exit();
    }
} else {
    $_SESSION['noauthenticado'] = true;
    header('Location: ./');
    exit();
}