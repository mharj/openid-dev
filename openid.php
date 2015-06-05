<?php                                                                                                                                                                                                                                             
define('CLIENTID','');                                                                                                                                                                                        
if ( session_id() === '' ) session_start();                                                                                                                                                                                                       
class OpenIDException extends \Exception {}                                                                                                                                                                                                       
                                                                                                                                                                                                                                                  
class OpenID {                                                                                                                                                                                                                                    
        private $domain = null;                                                                                                                                                                                                                   
        private $oid_config = null;                                                                                                                                                                                                               
        private $redirect_url = null;                                                                                                                                                                                                             
        private $id_body = null;                                                                                                                                                                                                                  
        // handle metadata and POST login data handling for login                                                                                                                                                                                 
        public function __construct($domain,$redirect_url=null) {                                                                                                                                                                                 
                if ( $redirect_url == null ) {
                        $redirect_url = (isset($_SERVER['HTTPS'])?'https':'http').'://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
                }
                $this->domain = $domain;
                $this->redirect_url = $redirect_url;
                $this->getMetadata();
                if ( isset($_POST['id_token']) ) {
                        $token_data  = explode(".", $_POST['id_token']);
                        $_SESSION['__ID_BODY']=base64_decode($token_data[1]);
                        header('Location: '.$this->redirect_url); // we are ok, back to original page
                        exit;
                }
                if ( isset($_SESSION['__ID_BODY']) ) {
                        $this->id_body = json_decode($_SESSION['__ID_BODY']);
                }
        }
        // store OpenID connection configuration to session, so only need to use this when new session happens
        public function getMetadata() {
                if ( ! isset($_SESSION['_oid_config'] ) ) {
                        $_SESSION['_oid_config']=$this->curlLoader($this->domain."/.well-known/openid-configuration");
                }
                $this->oid_config = json_decode($_SESSION['_oid_config']);
        }
        // auth redirect to service
        public function doAuth() {
                if ( time() > $this->id_body->exp ) {
                        $URL=$this->oid_config->authorization_endpoint."?response_type=id_token&client_id=".CLIENTID."&scope=openid&nonce=".session_id()."&response_mode=form_post&redirect_uri=".urlencode($this->redirect_url);
                        header('Location: '.$URL);
                        exit;
                }
        }
        // check if user is still logged
        public function isLogged() {
                if ( $this->id_body != null && time() > $this->id_body->exp ) {

                }
                return ($this->id_body == null?false:true);
                return false;
        }
        // read id_body attribute
        public function getAttribute($attr) {
                return $this->id_body->$attr;
        }
        // curl wrapper
        private function curlLoader($url) {
                $process = curl_init($url); 
                curl_setopt($process, CURLOPT_RETURNTRANSFER, 1);
                curl_setopt($process, CURLOPT_TIMEOUT, 10);
                curl_setopt($process, CURLOPT_FAILONERROR,true);
                $return = curl_exec($process);
                if( $return === false ) {
                        throw new OpenIDException(curl_error($process));
                }                                                                                                                                                                                                                                                            
                curl_close($process);                                                                                                                                                                                                                                        
                return $return;                                                                                                                                                                                                                                              
        }                                                                                                                                                                                                                                                                    
}                                                                                                                                                                                                                                                                            
