<?php

function phone_send_form($form, &$form_state) {
  $form['service'] = array(
    '#type' => 'select',
    '#required' => 'required',
    '#title' => t('Phone send service'),
    '#description' => t('Phone send service'),
    '#options' => phone_send_service_names(),
    '#empty' => variable_get('phone_send_service_default'),
  );
  $form['phone'] = array(
    '#type' => 'textarea',
    '#required' => 'required',
    '#title' => t('Phone number'),
    '#description' => '请输入手机号码，多个手机号码请使用 , 分隔',
    '#rows' => 1,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#required' => 'required',
    '#title' => t('Message'),
    '#description' => t('Phone message content'),
    '#rows' => 3
  );
  $form['mode'] = array(
    '#type' => 'radios',
    '#required' => 'required',
    '#title' => t('Phone mode'),
    '#options' => array('0' => 'duly', '1' => 'delay'),
    '#default_value' => 0
  );
  $form['cron'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone cron'),
    '#states' => array(
      'enabled' => array(
        ':input[name="mode"]' => array('value' => '1'),
      ),
      'required' => array(
        ':input[name="mode"]' => array('value' => '1'),
      ),
    )
  );
  if (module_exists('date_popup')) {
    $form['cron']['#type'] = 'date_popup';
    $form['cron']['#date_format'] = 'Y-m-d H:i:s';
  } else {
    $form['cron']['#description'] = '例如： ' . date('Y-m-d H:i:s');
  }
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Send')
    )
  );
  $form['#validate'] = array('phone_send_form_validate');
  $form['#submit'] = array('phone_send_form_submit');
  return $form;
}

function phone_send_form_validate($form, &$form_state) {
  $phones = explode(',', $form_state['values']['phone']);
  foreach ($phones as &$phone) {
    if (!phone_send_verify($phone, $form_state['values']['service'])) {
      form_set_error('phone', 'phone number verify is pass');
    }
  }
  $form_state['values']['phones'] = $phones;
}

function phone_send_form_submit($form, &$form_state) {
  $phones = $form_state['values']['phones'];
  if (empty($form_state['values']['mode'])) {
    $success = $fail = array();
    foreach ($phones as $phone) {
      $isSend = phone_send($phone, $form_state['values']['message'], $form_state['values']['service']);
      if ($isSend) {
        $success[] = $phone;
      } else {
        $fail[] = $phone;
      }
    }
    $message = ($success ? t('phone number @success send success', array('@success' => implode(',', $success))) : '') .
        ( $fail ? t('phone number @fail send fail', array('@fail' => implode(',', $fail))) : '');
    drupal_set_message($message);
  } else {
    
  }
}
