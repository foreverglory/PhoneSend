<?php

class PhoneSendOAuth implements HttpClientAuthentication {

  private $config;

  public function __construct($config) {
    $this->config = $config;
  }

  public function authenticate($request) {
    $request->parameters = $this->config['parameters'];
    $request->options['curlopts'] = $this->config['curlopts'];
    $request->data = http_build_query($this->config['data']);
  }

}
