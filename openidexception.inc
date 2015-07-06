<?php
namespace openid;

class OpenIDException extends \Exception {
	private $object = null;
	public function __construct($msg,$object) {
		parent::__construct($msg);
		$this->object = $object;
	}
	public function getObject() {
		return $this->object;
	}
} 