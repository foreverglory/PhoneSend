<?php

class PhoneSend {

  private $service = array();

  public function __construct($service) {
    $this->setService($service);
  }

  public function setService($service) {
    $this->service = $service;
    return $this;
  }

  public function getService($field = NULL) {
    if (empty($field)) {
      return $this->service;
    }
    $value = $this->service;
    foreach ((array) $field as $key) {
      $value = $value[$key];
    }
    return $value;
  }

  public function verify(&$phone) {
    $phone = trim($phone);
    return $phone && (empty($this->service['verify']) ? TRUE : preg_match($this->service['verify'], $phone));
  }

  public function send($phone, $message) {
    $oAuthConfig = $this->buildOAuth($phone, $message, $this->service['oauth']);
    $oAuth = new PhoneSendOAuth($oAuthConfig);
    $client = new HttpClient($oAuth);
    $request = new HttpClientRequest($this->service['url'], array(
      'method' => strtoupper($this->service['method']),
    ));
    $result = $client->execute($request);
    $isSend = $this->verifyResult($result, $this->service['result']);
    return $isSend;
  }

  private function verifyResult($result, $config) {
    $checked = FALSE;
    switch ($config['format']) {
      case 'html':
        $checked = $result == $config['success'] ? TRUE : FALSE;
        break;
      case 'json':
        $data = drupal_json_decode($result);
        list($keys, $val) = explode('=', $config['success']);
        foreach (explode('.', $keys) as $key) {
          if (!array_key_exists($key, $data)) {
            break 2;
          }
          $data = $data[$key];
        }
        $checked = $data == $val ? TRUE : FALSE;
        break;
      case 'xml':
        break;
    }
    return $checked;
  }

  private function buildOAuth($phone, $message, $config) {
    $search = array('[phone]', '[message]');
    $replace = array($phone, $message);
    $build = $this->array_replace($search, $replace, $config);
    return $build;
  }

  private function array_replace($search, $replace, $config) {
    if (is_array($config)) {
      foreach ($config as $key => $val) {
        $config[$key] = $this->array_replace($search, $replace, $val);
      }
    } elseif (is_string($config)) {
      $config = str_replace($search, $replace, $config);
    }
    return $config;
  }

}
