<?php

$hostname = 'localhost';  // substitua pelo nome do servidor do seu banco de dados
$username = 'root';  // substitua pelo nome de usuário do seu banco de dados
$password = '';  // substitua pela senha do seu banco de dados
$database = 'bypass';  // substitua pelo nome do seu banco de dados

// Conexão com o banco de dados
$connection = mysqli_connect($hostname, $username, $password, $database);

if (!$connection) {
    die("Falha na conexão com o banco de dados: " . mysqli_connect_error());
}

// Realizar a consulta na tabela loaderstatus
$query = "SELECT status FROM loaderstatus";
$result = mysqli_query($connection, $query);

if ($result) {
    // Verificar se há registros retornados
    if (mysqli_num_rows($result) > 0) {
        $row = mysqli_fetch_assoc($result);
        $status = $row['status'];

        // Verificar o valor da coluna status
        if ($status === "on") {
            echo "API:LOADER_ONLINE";
        } else {
            echo "API:LOADER_OFFLINE";
        }
    } else {
        echo "Sem registros encontrados na tabela loaderstatus.";
    }
} else {
    echo "Erro ao consultar a tabela loaderstatus: " . mysqli_error($connection);
}

// Fechar a conexão com o banco de dados
mysqli_close($connection);
?>