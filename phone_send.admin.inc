<?php

function phone_send_curl_opt_string($defined = NULL) {
  $defineds = drupal_static(__FUNCTION__, array());
  if (empty($defineds)) {
    foreach (get_defined_constants() as $key => $global) {
      if (strpos($key, 'CURLOPT_') === 0) {
        $defineds[$global] = $key;
      }
    }
  }
  if ($defined == NULL) {
    return $defineds;
  }
  return empty($defineds[$defined]) ? FALSE : $defineds[$defined];
}

function phone_send_settings($form, &$form_state) {
  $form['phone_send_service_add'] = array(
    '#type' => 'button',
    '#value' => t('Add Phone Send Service')
  );
  $header = array(
    'machine_name' => t('machine_name'),
    'title' => t('Title'),
    'url' => t('Url'),
    'method' => t('Method'),
    'oauth' => t('oAuth'),
    'result' => t('Result'),
    'op' => array('data' => t('operate')),
  );
  $options = array();
  foreach (phone_send_phone_send_service() as $name => $service) {
    $options[$name] = array(
      'machine_name' => $name,
      'title' => $service['title'],
      'url' => $service['url'],
      'method' => $service['method'],
      'oauth' => t('oAuth'),
      'result' => t('Result'),
      'op' => array(
        'data' => array(
          array(
            '#type' => 'link',
            '#title' => '编辑',
            '#href' => 'admin'
          ),
          array(
            '#type' => 'link',
            '#title' => '删除',
            '#href' => 'admin'
          )
        )
      )
    );
  }
  $form['phone_send_service'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
  );
  return system_settings_form($form);
}

function phone_send_service_settings($form, &$form_state) {
  $form['title'] = array(
    ''
  );
  $form['curlopts'] = array(
    '#parents' => array('phone_send_service', 'curlopts'),
    '#type' => 'select',
    '#options' => phone_send_curl_opt_string(),
  );
  return system_settings_form($form);
}
