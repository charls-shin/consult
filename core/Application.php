<?php


namespace consult\core;



class Application
{
	public static $ROOT_DIR;
	public static $app;
	public $session;
	public $router;
	public $controller;
	public $view;
	

	protected $controllMap=[];

	public function __construct($rootPath,array $config=array())
	{
		self::$ROOT_DIR=$rootPath;
		$this->router=new Router();
		$this->session=new Session();
		$this->view=new View();
		$this->controller=$this->getController();
		self::$app = $this;
	}
	
	private function getController()
	{
		$ctrlFile=$this->getCtrlFile();
		
		if( !file_exists($_SERVER['DOCUMENT_ROOT'].$ctrlFile) ){
			echo $this->view->renderOnlyView('_404',['msg'=>'페이자가 없습니다','path'=>$_SERVER['DOCUMENT_ROOT'].$ctrlFile]);
			exit();
		}
		$ctrl=str_replace(["/",".php"], ["\\",""], $ctrlFile);
		
		return new $ctrl();
	}
	
	public function getCtrlFile()
	{
		$rPath=$this->router::$path;
		$rFn=$this->router::$fn;
		$aPath=explode('/',$rPath);
		$ctrlNm=array_pop($aPath);
		if( $ctrlNm== ''){
			$ctrlNm=$rFn;
		}
		if (!empty($aPath)) {
			$subPath=implode('/',$aPath).'/';
		}else{
			$subPath='';
		}
		
		$path=PATH_SUB.'controllers/';
		$ctrlNm=ucfirst(strtolower($ctrlNm));
		$ctrlFile="{$path}{$subPath}{$ctrlNm}Controller.php";
		
		return $ctrlFile;
	}

	public function run()
	{
		echo $this->view->getContent($this->controllMap);
	}

	public function setControlls($callback)
	{
		$this->controllMap[]=$callback;
	}
}