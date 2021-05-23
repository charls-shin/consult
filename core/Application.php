<?php


namespace consult\core;



class Application extends Common
{
	public static $ROOT_DIR;
	public static $app;
	/** @var Session $session */
	public $session;
	/** @var Router $router */
	public $router;
	/** @var View $view */
	public $view;
	public $controller;
	

	protected $controllMap=[];

	public function __construct($rootPath,array $config=array())
	{
		parent::__construct();
		self::$ROOT_DIR=$rootPath;
		$this->router=new Router();
		$this->session=new Session();
		$this->view=new View();
		$this->controller=$this->getController();
		self::$app = $this;
	}

	public function run()
	{
		$fn=$this->router::$fn;
		if( $this->router->isPost() ){
			$fn.='Action';
		}
		if( !method_exists($this->controller,$fn) ){
			echo $this->view->renderOnlyView('_404',['msg'=>'페이자가 없습니다','path'=>$fn]);
			exit();
		}
		
		echo call_user_func([$this->controller,$fn],$this->router->getBody());
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
	
	private function getCtrlFile()
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
		
		return "{$path}{$subPath}{$ctrlNm}Controller.php";
	}
}