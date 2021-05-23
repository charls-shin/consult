<?php


namespace consult\core;


class Controller
{
	public $layout = 'main';
	
	public function setLayout($layout)
	{
		$this->layout = $layout;
	}
	
	public function render($view,$params=[])
	{
		return Application::$app->view->renderView($view,$params);
	}
	
	public function layout_head($params=[])
	{
		$params['hTitle']='���ջ�� �ַ�� ������';
		//return Application::$app->view->renderOnlyView("layouts/{$this->layout}/head",$params);
		return Application::$app->view->renderOnlyView("layout_head",$params);
	}
}