<?php
namespace openid;

interface OpenIDLogInterface {
	public function clear();
	public function debug($msg,$object);
	public function error($msg,$object);
}
