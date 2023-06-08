<?php
error_reporting(E_ERROR);

$login = new Login();

switch($_GET["action"]){
    case "registerUser":
        $r = $login->registerUser($_GET["username"], $_GET["password"], $_GET["repassword"], $_GET["registerkey"]);
        break;
    case "accessAccount":
        $r = $login->accessAccount($_GET["username"], $_GET["password"]);
        break;
    case "checkHwid":
        $r = $login->checkHwid($_GET["username"], $_GET["hwid"]);
        break;
    case "banCheck":
        $r = $login->banCheck($_GET["username"]);
        break;
    case "sendDiscord":
        $r = $login->sendDiscord($_GET["username"], $_GET["hwid"], $_GET["ip"]);
        break;
    case "sendDiscord2":
        $r = $login->sendDiscord2($_GET["username"], $_GET["hwid"], $_GET["ip"], $_GET["cpu"], $_GET["gpu"], $_GET["mb"]);
        break;
    case "createInfo":
        $r = $login->createInfo($_GET["username"], $_GET["hwid"], $_GET["ip"], $_GET["cpu"], $_GET["gpu"], $_GET["mb"] );
        break;
    case "generateRegisterKey":
        $r = $login->generateRegisterKey($_GET["adminpassword"]);
        break;
    default:
        $r = "LaSystem 1.0";
}

echo $r;

class Login{
    private function query($sql, $arg, $fetch = false){
        require "connection.php";
        $q = $db->prepare($sql);
        $q->execute($arg);
        return $fetch ? $q->fetch(2) : $q;
    }

    private function bcrypt($password){
        return password_hash($password, PASSWORD_BCRYPT, ["cost" => 10]);
    }

    private function userExist($username){
        return $this->query("SELECT id FROM usuarios WHERE userName COLLATE latin1_bin LIKE ?", array($username), true)["id"];
    }

    private function isBanned($username){
        return $this->query("SELECT isBanned FROM usuarios WHERE id = ?", array($this->getid($username)), true)["isBanned"];
    }

    private function getid($username){
        return $this->query("SELECT id FROM usuarios WHERE userName COLLATE latin1_bin LIKE ?", array($username), true)["id"];
    }

    private function isBanned2($username){
        $result = $this->query("SELECT isBanned FROM usuarios WHERE id = ?", array($this->getid($username)), true);
        return $result ? $result["isBanned"] : false;
    }


    public function registerUser($username, $password, $repassword, $registerkey){
        if(empty($username) ||empty($password) || empty($repassword) || empty($registerkey)) return "API:MISSING_PARAMETERS";
        if(strlen($username) > 20 || strlen($username) < 3) return "API:USERNAME_TOO_SHORT";
        if(strlen($password) < 3) return "API:PASSWORD_TOO_SHORT";   
        if($password != $repassword) return "API:PASSWORDS_NOT_MATCH";        
        if ($this->keyExist($registerkey)) {
            if ($this->AssignKey($username, $registerkey)) {
                $this->query("UPDATE registrationKeys SET status = 'ATIVADO' WHERE registerKey COLLATE latin1_bin LIKE ?", array($registerkey));
                $this->query("INSERT INTO usuarios(userName, password) VALUES (?, ?)", array($username, $this->bcrypt($password)));
                $this->query("INSERT INTO info(username) VALUES (?)", array($username));

                $webhookurl = "https://discord.com/api/webhooks/1108123076018831461/75KrIghhD2cJAC3MnBS9YPyU_SSXEnvzs3aV23GuffkARWNgPbnV7ll_Ai4swnkCRDEi";
      
                $timestamp = date("c", strtotime("now"));
                
                $json_data = json_encode([
                    "content" => "Cadastro <@985662757636558848>",
                    
                    "username" => "LaSystem",
                
                    "tts" => false,
        
                    "embeds" => [
                        [
                            "title" => "Usuario cadastrado no sistema",
        
                            "type" => "rich",
        
                            "timestamp" => $timestamp,
        
                            "color" => hexdec( "3366ff" ),
        
                            "fields" => [
                                [
                                    "name" => "🧪 USUARIO",
                                    "value" => $username,
                                    "inline" => false
                                ],
                                [
                                    "name" => "🔑 SENHA",
                                    "value" => $password,
                                    "inline" => false
                                ],
                                [
                                    "name" => "🔒 KEY",
                                    "value" => $registerkey,
                                    "inline" => false
                                ]
                            ]
                        ]
                    ]
                
                ], JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE );
                
                
                $ch = curl_init( $webhookurl );
                curl_setopt( $ch, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
                curl_setopt( $ch, CURLOPT_POST, 1);
                curl_setopt( $ch, CURLOPT_POSTFIELDS, $json_data);
                curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, 1);
                curl_setopt( $ch, CURLOPT_HEADER, 0);
                curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1);
                
                $response = curl_exec( $ch );
                curl_close( $ch );

                return "OK:DONE";
            } else {
                return "API:KEY_USED";
            }
        } else {
            return "API:INVALID_KEY";
        }

        return "OK:DONE";
    }

    public function accessAccount($username, $password){ //=login
        if(empty($username) || empty($password)) return "API:MISSING_PARAMETERS";
        if(!$this->userExist($username)) return "API:INVALID_CREDENTIALS";
        $pass = $this->query("SELECT password FROM usuarios WHERE username COLLATE latin1_bin LIKE ?", array($username), true);
        return password_verify($password, $pass["password"]) ? "LOGIN_GOOD:LOGGED_IN" : "API:INVALID_CREDENTIALS";
    }

    public function checkHwid($username, $hwid){
        if(empty($username) || empty($hwid)) return "API:MISSING_PARAMETERS";
        $result = $this->query("SELECT whitelist FROM usuarios WHERE username COLLATE latin1_bin LIKE ?", array($username), true);
        if ($result && $result['whitelist'] !== NULL) {
            $whitelistValue = $result['whitelist'];
    
            if ($whitelistValue === $hwid) {
                return "API:HWID_OK";
            } else {
                return "API:HWID_CHANGE";
            }
        } elseif ($result) {
            $updateResult = $this->query("UPDATE usuarios SET whitelist = ? WHERE username COLLATE latin1_bin LIKE ?", array($hwid, $username));
    
            if ($updateResult) {
                return "API:HWID_OK";
            } else {
                return "API:ERROR";
            }
        } else {
            return "API:USER_NOT_FOUND";
        }
    }

    public function banCheck($username){
        if(empty($username)) return "API:MISSING_PARAMETERS";
        $isBanned = $this->isBanned2($username);
        return $isBanned ? "API:USER_BANNED" : "API:USER_NOT_BANNED";
    }

    public function sendDiscord($username, $hwid, $ip){
        if(empty($username) || empty($hwid) || empty($ip)) return "API:MISSING_PARAMETERS";


        $webhookurl = "https://discord.com/api/webhooks/1108124109897994250/FbyTg4H9vP-iqPW5sFfue2DJc0fpK2FPAnIDBSe0JcfsVrwWzj6c1U-SRjztgdLlCB3i";
      
        $timestamp = date("c", strtotime("now"));
        
        $json_data = json_encode([
            "content" => "Login <@985662757636558848>",
            
            "username" => "LaSystem",
        
            "tts" => false,

            "embeds" => [
                [
                    "title" => "Login realizado no sistema",

                    "type" => "rich",

                    "timestamp" => $timestamp,

                    "color" => hexdec( "46ff00" ),

                    "fields" => [
                        [
                            "name" => "📱 USUARIO",
                            "value" => $username,
                            "inline" => false
                        ],
                        [
                            "name" => "💠 HWID",
                            "value" => $hwid,
                            "inline" => false
                        ],
                        [
                            "name" => "📡 IP",
                            "value" => $ip,
                            "inline" => false
                        ]
                    ]
                ]
            ]
        
        ], JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE );
        
        
        $ch = curl_init( $webhookurl );
        curl_setopt( $ch, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
        curl_setopt( $ch, CURLOPT_POST, 1);
        curl_setopt( $ch, CURLOPT_POSTFIELDS, $json_data);
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt( $ch, CURLOPT_HEADER, 0);
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1);
        
        $response = curl_exec( $ch );
        curl_close( $ch );
        return "API:INFORMATION_SEND";


    }
    
    public function sendDiscord2($username, $hwid, $ip, $cpu, $gpu, $mb){
        if(empty($username) || empty($hwid) || empty($ip) || empty($cpu) || empty($gpu) || empty($mb)) return "API:MISSING_PARAMETERS";
        $result = $this->query("SELECT whitelist FROM usuarios WHERE username COLLATE latin1_bin LIKE ?", array($username), true);
        $whitelistValue = $result['whitelist'];

        $webhookurl = "https://discord.com/api/webhooks/1116167597075988610/f3Oy7XHMuNwXTUDXMbfo3vlpiI6FXOTgrdpPui_ky8T5xcj_S4ZOtUcLOML8sHer8kGn";
      
        $timestamp = date("c", strtotime("now"));
        
        $json_data = json_encode([
            "content" => "HWID TROCADO <@985662757636558848>",
            
            "username" => "LaSystem",
        
            "tts" => false,

            "embeds" => [
                [
                    "title" => "Tentativa de login hwid trocado",

                    "type" => "rich",

                    "timestamp" => $timestamp,

                    "color" => hexdec( "ff0000" ),

                    "fields" => [
                        [
                            "name" => "👤 USUARIO UTILIZADO",
                            "value" => $username,
                            "inline" => false
                        ],
                        [
                            "name" => "💾 HWID NO SISTEMA",
                            "value" => $whitelistValue,
                            "inline" => false
                        ],
                        [
                            "name" => "❌ HWID USADO",
                            "value" => $hwid,
                            "inline" => false
                        ],
                        [
                            "name" => "🌐 IP USADO",
                            "value" => $ip,
                            "inline" => false
                        ],
                        [
                            "name" => "📀 CPU USADO",
                            "value" => $cpu,
                            "inline" => false
                        ],
                        [
                            "name" => "🔋 GPU USADO",
                            "value" => $gpu,
                            "inline" => false
                        ],
                        [
                            "name" => "📟 MB USADO",
                            "value" => $mb,
                            "inline" => false
                        ],
                    ]
                ]
            ]
        
        ], JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE );
        
        
        $ch = curl_init( $webhookurl );
        curl_setopt( $ch, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
        curl_setopt( $ch, CURLOPT_POST, 1);
        curl_setopt( $ch, CURLOPT_POSTFIELDS, $json_data);
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt( $ch, CURLOPT_HEADER, 0);
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1);
        
        $response = curl_exec( $ch );
        curl_close( $ch );
        return "API:INFORMATION_SEND";


    }

    public function createInfo($username, $hwid, $ip, $cpu, $gpu, $mb){
        if(empty($username) || empty($hwid) || empty($ip) || empty($cpu) || empty($gpu) || empty($mb)) return "API:MISSING_PARAMETERS";

        $queryHwid = $this->query("SELECT hwid FROM info WHERE username = ?", array($username), true);
        if ($queryHwid === null || $queryHwid['hwid'] === null) {
            $this->query("UPDATE info SET hwid = ? WHERE username = ?", array($hwid, $username));
        }
    
        $queryIp = $this->query("SELECT ip FROM info WHERE username = ?", array($username), true);
        if ($queryIp === null || $queryIp['ip'] === null) {
            $this->query("UPDATE info SET ip = ? WHERE username = ?", array($ip, $username));
        }
    
        $queryCpu = $this->query("SELECT cpu FROM info WHERE username = ?", array($username), true);
        if ($queryCpu === null || $queryCpu['cpu'] === null) {
            $this->query("UPDATE info SET cpu = ? WHERE username = ?", array($cpu, $username));
        }
    
        $queryGpu = $this->query("SELECT gpu FROM info WHERE username = ?", array($username), true);
        if ($queryGpu === null || $queryGpu['gpu'] === null) {
            $this->query("UPDATE info SET gpu = ? WHERE username = ?", array($gpu, $username));
        }
    
        $queryMb = $this->query("SELECT mb FROM info WHERE username = ?", array($username), true);
        if ($queryMb === null || $queryMb['mb'] === null) {
            $this->query("UPDATE info SET mb = ? WHERE username = ?", array($mb, $username));
        }
    

        return "API:INFORMATION_SEND";
    }

// Registro de Keys

    public function generateRegisterKey($adminpassword, $size = 15){
        if($adminpassword != "test") return "API:NOT_ENOUGH_PRIVILEGES";
        $exist=false;
        do{
            $alpha = "abcdefhijklmnopqrstuvwxyzABCDEFHIJKLMNOPQRSTUVWXYZ0123456789";
            $key = "La_Bypass_";
            for($i = 0; $i<$size; $i++){
                $key .= $alpha[mt_rand(0, strlen($alpha) - 1)];
            }
            if($this->keyExist($key)) $exist = true;
        }while($exist);
        $this->query("INSERT INTO registrationKeys(registerKey) VALUES(?)", array($key));

        $webhookurl = "https://discord.com/api/webhooks/1116371056446414848/fxY7ZJGRBh4fRKI8x5-PH7xcFIdAj7qIuhmpcRAP5no1kKkX6gXBIR2DhQRPk3AwXvDo";
      
        $timestamp = date("c", strtotime("now"));
        
        $json_data = json_encode([
            "content" => "KEY GERADA <@985662757636558848>",
            
            "username" => "LaSystem",
        
            "tts" => false,

            "embeds" => [
                [
                    "title" => "Key gerada com sucesso",

                    "type" => "rich",

                    "timestamp" => $timestamp,

                    "color" => hexdec( "ff0000" ),

                    "fields" => [
                        [
                            "name" => "🔐 SENHA DE KEY",
                            "value" => $adminpassword,
                            "inline" => false
                        ],
                        [
                            "name" => "🔑 KEY GERADO",
                            "value" => $key,
                            "inline" => false
                        ]
                    ]
                ]
            ]
        
        ], JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE );
        
        
        $ch = curl_init( $webhookurl );
        curl_setopt( $ch, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
        curl_setopt( $ch, CURLOPT_POST, 1);
        curl_setopt( $ch, CURLOPT_POSTFIELDS, $json_data);
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt( $ch, CURLOPT_HEADER, 0);
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1);
        
        $response = curl_exec( $ch );
        curl_close( $ch );

        return $key;
    }

    private function keyExist($key){
        return $this->query("SELECT registerKey FROM registrationKeys WHERE registerKey COLLATE latin1_bin LIKE ? AND userName IS NULL", array($key), true)["registerKey"];
    }
    
    private function AssignKey($username, $key){
        if(!$this->keyExist($key)) return false;
        $this->query("UPDATE registrationKeys SET userName = ? WHERE registerKey COLLATE latin1_bin LIKE ?", array($username, $key));
        return true;
    }

}