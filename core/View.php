<?php


namespace consult\core;


class View
{
	public $html='';
	public $layout = 'main';

	public function setLayout($layout)
	{
		$this->layout=$layout;
	}
	public function renderOnlyView($view,$params)
	{
		foreach( $params as $key=>$value ){
			$$key=$value;
		}
		
		ob_start();
		include_once Application::$ROOT_DIR."views/$view.php";
		return ob_get_clean();
	}

	public function renderView($view,$params = [])
	{
		$viewContent=$this->bodyContent($view,$params);
		return str_replace("{{$view}}",$viewContent,$this->html);
	}

	public function layoutContent()
	{
		$layout=$this->layout;
		ob_start();
		include_once Application::$ROOT_DIR."views/layouts/$layout.php";
		return ob_get_clean();
	}

	public function bodyContent($view,$params)
	{
		foreach( $params as $key=>$value ){
			$$key=$value;
		}
		ob_start();
		include_once Application::$ROOT_DIR."views/$view.php";
		return ob_get_clean();
	}


	public function getContent(array $controllMap)
	{
		$this->html=$this->layoutContent();
		
		$controllMap=$this->getReCallbackMap($controllMap);

		foreach ($controllMap as $callback)
		{
			$controller = new $callback[0];
			Application::$app->controller = $controller;
			$callback[0] = $controller;
			$this->html=call_user_func($callback);
		}
		return $this->html;
	}
	
	public function getReCallbackMap(array $controllMap)
	{
		if( $this->html == '' ) return $controllMap;
		
		preg_match_all('/{(layout_[a-z_][\w]+)}/', $this->html, $matches);
		
		
		print "<pre>";
		print_r($matches);
		print_r($controllMap);
		print "</pre>";
		die();
		
		
		
		return $controllMap;
	}
}