<?php
$br = (php_sapi_name() == "cli")? "\n":"<br>";
$module = 'stwms';
//加载模块
if(!extension_loaded($module)) {
	dl($module . PHP_SHLIB_SUFFIX);
}

//输出模块可用函数
$functions = get_extension_funcs($module);
echo '模块'.$module.'可用函数：'.$br;
echo implode(', ',$functions).$br;

class Book{
	
}

function showMessage(){
	var_dump($GLOBALS['phpvar']);
}

if (extension_loaded($module)){
	
//	$arr = array(0 => '0',1 => '123','a' => 'abc',);  
//	$prefix = array(1 => '456','a' => 'def',);  
//	var_dump(stwms_array_concat($arr, $prefix));	
	
	
	//获取ini变量
	$stwms_name=ini_get('stwms.name'); 
	
	$phpvar=false;
	//var_dump(isset($hello)); 
	//stwms_test函数测试
	var_dump(stwms_test($stwms_name));
	//var_dump(stwms_makecode('spring'));
	//showMessage();
	//$hello='xiechunping';
	
	//stwms_makecode函数测试
	//echo 'stwms_makecode:';
	//var_dump(stwms_makecode('spring'));
	
//	stwms_var_dump函数测试
//	$book=new Book();
//	$array=array('name'=>'spring','age'=>22,'money'=>11.11,'gender'=>true,'book'=>$book);
//	stwms_var_dump($array);
}
?>
