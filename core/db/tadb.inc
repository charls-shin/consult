<?
	include_once PATH_SUB_ROOT.'lib/tadb_mts.inc';

	//include $_SERVER['DOCUMENT_ROOT'].'admin/lib/tadb_sqlrelay.inc';
	
	//////// Object »ý¼º ////////////////////////////////
	if(!is_object($oci)){
		$oci = new Oracle_OCI;				
	}
	