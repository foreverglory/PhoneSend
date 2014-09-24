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

function _phone_send_setting_options($name = Null, $service = array()) {
  $form = array();
  $form[$name] = array(
    '#parents' => array('phone_send_service'),
    '#type' => 'fieldset',
    '#title' => is_null($name) ? t('Add') : $service['title'],
    '#group' => 'settings',
    'options' => array(
      'title' => array(
        '#parents' => array('phone_send_service', $name, 'title'),
        '#title' => t('title'),
        '#type' => 'textfield',
        '#default_value' => $service['title']
      ),
      'machine_name' => array(
        '#type' => 'machine_name',
        '#default_value' => $name,
        '#machine_name' => array(
          'exists' => 'title',
          'replace_pattern' => '[^a-z0-9-]+',
          'replace' => '-',
        ),
        '#disabled' => !empty($service['title']),
      )
    )
  );
  return $form;
}

function phone_send_settings($form, &$form_state) {
  $form['phone_send_service_add'] = array(
    '#type' => 'button',
    '#value' => t('Add Phone Send Service')
  );
  $header = array(
    'title' => array('data' => t('Title')),
    'url' => t('Url'),
    'method' => t('Method'),
    'oauth' => t('oAuth'),
    'result' => t('Result'),
    'op' => array('data' => t('operate')),
  );
  $options = array();
  foreach (phone_send_phone_send_service() as $name => $service) {
    $options[$name] = array(
      'title' => $service['title'],
      'url' => $service['url'],
      'method' => $service['method'],
      'oauth' => var_export($service['oauth'], true),
      'result' => var_export($service['result'], true),
      'op' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => '编辑',
          '#href' => 'admin/config/system/phone-send/form/' . $name
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
  $form['actions']['delete'] = array(
    '#type' => 'button',
    '#value' => t('delete selected')
  );
  return system_settings_form($form);
}

function phone_send_settings_validate($form, &$form_state) {
  
}

function phone_send_settings_form($form, &$form_state) {
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
