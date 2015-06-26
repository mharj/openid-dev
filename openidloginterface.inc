<?php
namespace openid;

interface OpenIDLogInterface {
	public function debug($msg);
	public function error($msg);
}
