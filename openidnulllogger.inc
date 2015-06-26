<?php
namespace openid;

class OpenIDNullLogger implements OpenIDLogInterface {
	public function debug($msg) {}
	public function error($msg) {}
}
