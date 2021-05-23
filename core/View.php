<?php


namespace consult\core;


class View
{
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
		$layoutContent=$this->layoutContent();
		
		$path=Application::$app->router::$path;
		$path=($path!='' ) ? '/': '';
		$viewContent=$this->renderOnlyView($path.$view,$params);
		$html=str_replace("{layout_content}",$viewContent,$layoutContent);
		
		$callBacks=$this->getLayoutCallBack($html);
		
		foreach ($callBacks as $callBack){
			$callbackView=call_user_func([Application::$app->controller,$callBack],$params);
			$html=str_replace("{{$callBack}}",$callbackView,$html);
		}
		
		return $html;
	}

	public function layoutContent()
	{
		$layout=Application::$app->controller->layout;
		ob_start();
		include_once Application::$ROOT_DIR."views/layouts/$layout.php";
		return ob_get_clean();
	}
	
	
	public function getLayoutCallBack($html)
	{
		preg_match_all('/{(layout_[a-z_][\w]+)}/', $html, $matches);
		return $matches[1];
	}
}