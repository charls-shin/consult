<?php


namespace consult\core;


class Controller
{
	
	public function __construct()
	{
	
	}

	public function render($view,$params=[])
	{
		return Application::$app->view->renderView($view,$params);
	}
}