<?php
namespace openid;

class OpenIDNullLogger implements OpenIDLogInterface {
	public function clear() {}
	public function debug($msg,$object) {}
	public function error($msg,$object) {}
}
