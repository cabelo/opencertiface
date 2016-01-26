<?php


/* Insira aqui a pasta que deseja salvar o arquivo*/
$uploaddir = './tmp/';

$uploadfile1 = $uploaddir . $_FILES['ufile1']['name'];
$uploadfile2 = $uploaddir . $_FILES['ufile2']['name'];

if (move_uploaded_file($_FILES['ufile1']['tmp_name'], $uploadfile1)){
echo "Arquivo Enviado ". $_FILES['ufile1']['name'] . " recebido com sucesso!"; }
else {echo "Arquivo nao enviado "; }
echo "<br>";

if (move_uploaded_file($_FILES['ufile2']['tmp_name'], $uploadfile2)){
echo "Arquivo Enviado ". $_FILES['ufile2']['name'] . " recebido com sucesso!"; }
else {echo "Arquivo nao enviado "; }
echo "<br>";

echo "<img src='$uploadfile1' />  <img src='$uploadfile2' />";

//$handle = fopen($uploadfile, "r");
//$contents = fread($handle, filesize($uploadfile));
//fclose($handle);

try {
$client = new SoapClient('http://opencertiface.com.br/opencertiface.wsdl', array('trace' => true, 'exceptions' => true));
$params1 = file_get_contents($uploadfile1);
$params2 = file_get_contents($uploadfile2);
//$params = array( 'imageBase64'	=> base64_encode(file_get_contents($uploadfile)));
echo "<br>";
$result = $client->verifyImg($params1,$params2, "00001" );
echo "Similaridade: " .  $result;

//var_dump($result);
}
catch (SoapFault $e) {
//echo "Erro";
 print $e->faultcode.' - '.$e->faultstring; 
//    var_dump($e);
}


?>
