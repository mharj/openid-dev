<?php
namespace openid;
                                                                                                                                                                                                                                                                                                                                                                                                                                                  
class OpenID {
	private $openIdConfig = null;
	private $oid_config = null;
	private $id_body = null;
	private $code = null;
                                                                                                                                                                         
	public function __construct(OpenIDConfig $openIdConfig) {
		if (  $openIdConfig == null ) {
			throw new OpenIDException("empty config!");
		}
		$this->openIdConfig = $openIdConfig;
		if (  session_id() === '' ) {
			throw new OpenIDException("http session not started!");
		}
		// use current url as redirect if not defined
		if ( $this->openIdConfig->getRedirectUri() == null ) {
			$this->openIdConfig->setRedirectUri( (isset($_SERVER['HTTPS'])?'https':'http').'://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'] );
		}
		$this->getMetadata();
	}

	// store OpenID connection configuration to session, so only need to use this when new session happens
	public function getMetadata() {
		if ( ! isset($_SESSION['__OPENID_CONFIG_METADATA'] ) ) {
			$_SESSION['__OPENID_CONFIG_METADATA']=$this->curlLoader( $this->openIdConfig->getDomainConfigurationUri() );
		}
		$this->oid_config = json_decode($_SESSION['__OPENID_CONFIG_METADATA']);
		if ( ! in_array($this->openIdConfig->getResponseType(),$this->oid_config->response_types_supported) ) {
			throw new OpenIDException("service is not supporting '".$this->openIdConfig->getResponseType()."' response type");
		}
	}
	
	// auth redirect to service
	public function doAuth() {
		$_SESSION['__OPENID_NONCE']=md5(session_id().time());
		$data=array("openid","email");
		$params = array(
			"response_type"	=> $this->openIdConfig->getResponseType(),
			"client_id"		=> $this->openIdConfig->getClientId(),
			"scope"			=> implode(" ",$data),
			"nonce"			=> $_SESSION['__OPENID_NONCE'],
			"response_mode"	=> "form_post",
			"redirect_uri"	=> $this->openIdConfig->getRedirectUri(),
		);
		$URL=$this->oid_config->authorization_endpoint."?".http_build_query($params);
		$_SESSION['__OPENID_REDIRECT']=(isset($_SERVER['HTTPS'])?'https':'http').'://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
		header('Location: '.$URL);
		exit;
	}
	// check if user is still logged and session is not expired
	public function isLogged() {
		if ( isset($_SESSION['__OPENID_PAYLOAD']) ) {
			$this->id_body = json_decode($_SESSION['__OPENID_PAYLOAD']);
			if ( time() < $this->id_body->exp ) {
				return true;
			}
		}
		return false;
	}
	// read id_body attribute
	public function getAttribute($attr) {
		return $this->id_body->$attr;
	}
	// curl wrapper
	private function curlLoader($url,array $header=null) {
		$process = curl_init($url); 
		curl_setopt($process, CURLOPT_RETURNTRANSFER, 1);
		if ( $header != null ) {
			curl_setopt($ch, CURLOPT_HTTPHEADER,$header);
		}
		if ( $this->openIdConfig->getHttpProxy() != null ) {
			curl_setopt($process, CURLOPT_PROXY, $this->openIdConfig->getHttpProxy() );
		}
		curl_setopt($process, CURLOPT_TIMEOUT, 10);
		curl_setopt($process, CURLOPT_FAILONERROR,true);
		$return = curl_exec($process);
		if( $return === false ) {
			throw new OpenIDException(curl_error($process));
		}                                                                                                                                                                                                                                                            
		curl_close($process);                                                                                                                                                                                                                                        
		return $return;                                                                                                                                                                                                                                              
	}

	public function validateToken(OpenIDToken $token) {
		$this->getMetadata();
		// 1. Verify that the ID token is a JWT which is properly signed with an appropriate public key.
		// TODO
		// 2. Verify that the value of aud in the ID token is equal to your app’s client ID.
		if ( $this->openIdConfig->getClientId() != $token->payload->aud ) {
			throw new OpenIDException("Can't validate token");
		}
		// 3. Verify that the value of iss in the ID token is equal to source
		if ( $this->oid_config->issuer != $token->payload->iss ) {
			throw new OpenIDException("Can't validate token");
		}
		// 4. Verify that the expiry time (exp) of the ID token has not passed.
		if ( time() > $token->payload->exp ) {
			throw new OpenIDException("Can't validate token");
		}
		// check nonce is matching
		if ( $token->payload->nonce != $_SESSION['__OPENID_NONCE'] ) {
			throw new OpenIDException("Can't validate token");
		}
		// store payload to session
		$_SESSION['__OPENID_PAYLOAD']=json_encode($token->payload);
		// redirect to original page
		header("Location: ". $_SESSION['__OPENID_REDIRECT']."\n");
		exit;
	}
}                                                                                                                                                                                                                                                                            
