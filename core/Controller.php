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
		$params['hTitle']='입합상담 솔루션 관리자';
		
		return Application::$app->view->renderOnlyView("layout_head",$params);
	}
	
	public function layout_sidebar($params=[])
	{
		return Application::$app->view->renderOnlyView("layout_sidebar",$params);
	}
	
	public function layout_topbar($params=[])
	{
		return Application::$app->view->renderOnlyView("layout_topbar",$params);
	}
	
	public function layout_footer($params=[])
	{
		return Application::$app->view->renderOnlyView("layout_footer",$params);
	}
	
	public function layout_logout($params=[])
	{
		return Application::$app->view->renderOnlyView("layout_logout",$params);
	}
	
	public function layout_javascript($params=[])
	{
		return Application::$app->view->renderOnlyView("layout_javascript",$params);
	}
}