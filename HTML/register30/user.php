<?php
	include_once("conexao.php");
	$nome_usuario = $_POST['txt_nome_usuario'];
	$email_usuario = $_POST['txt_email_usuario'];
	$key_usuario = $_POST['txt_key_usuario'];
	$ocaraefoda = $_POST['txt_select'];
	//echo "$nome_usuario - $email_usuario";
	
	
	$query = "SELECT * FROM `license_keys` WHERE license_key = '{$key_usuario}'";

$result = mysqli_query($conexao, $query);

$row = mysqli_num_rows($result);

        
	if(mysqli_affected_rows($conexao) != 0){
	if($row == 1)
	{

date_default_timezone_set('America/Sao_Paulo');

$expiry_time = date('Y-m-d H:i:s', strtotime('+30 days'));	
$result_usuario = "INSERT INTO apipremium(username, password, license_key, expiry_time, isPremium) VALUES ('$nome_usuario','$email_usuario','$key_usuario','$expiry_time','PREMIUM')";
$resultado_usuario = mysqli_query($conexao, $result_usuario);

$result_usuario = "UPDATE `license_keys` SET `license_key`='' WHERE license_key = '$key_usuario'";
$resultado_usuario = mysqli_query($conexao, $result_usuario);

    //read a file from source
    $src_image =   file_get_contents("https://cdn.discordapp.com/avatars/363106692038131712/31c5136575a29546bb7f8be9156c5af4.png");

    //give new file name
    $new_info = "../dashboard/uploaded_img/$nome_usuario.png";

    //write data to a file
    $status = file_put_contents($new_info, $src_image);


		echo "
		<META HTTP-EQUIV=REFRESH CONTENT = '0;URL=https://labypass.online/'>
		<script type=\"text/javascript\">
			alert(\"Usuario cadastrado com Sucesso.\");
		</script>
	";	
	session_destroy();
	header('Location: ../');
	}else{
		echo "
				<META HTTP-EQUIV=REFRESH CONTENT = '0;URL=https://labypass.online/register'>
				<script type=\"text/javascript\">
					alert(\"\");
				</script>
			";	
			header('Location: ../');
	}
	
	}
	

?>

