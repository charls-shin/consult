<?php


namespace consult\controllers;

use consult\core\Application;
use consult\core\Controller;

class IndexController extends Controller
{
	public function index($params)
	{
		return $this->render(Application::$app->router::$fn);
	}
}