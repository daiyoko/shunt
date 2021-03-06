<?php

/**
 * @file
 *   Shunt drush commands
 */

/**
 * Implements hook_drush_command().
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function shunt_drush_command() {
  $items = array();

  $items['shunt-enable'] = array(
    'description' => "Enable shunt(s)",
    'arguments' => array(
      'name' => 'The particular shunt to trip. Omit to choose from a list of options.',
    ),
    'examples' => array(
      'drush shunt-enable' => 'Enable ("trip") the shunt(s).',
      'drush se' => 'Enable ("trip") the shunt(s).',
    ),
    'aliases' => array('se'),
  );

  $items['shunt-disable'] = array(
    'description' => "Disable shunt(s)",
    'arguments' => array(
      'name' => 'The particular shunt to trip. Omit to choose from a list of options.',
    ),
    'examples' => array(
      'drush shunt-disable' => 'Enable ("trip") the shunt(s).',
      'drush sd' => 'Disable ("trip") the shunt(s).',
    ),
    'aliases' => array('sd'),
  );

  $items['shunt-get-enabled'] = array(
    'description' => "Get a list of enabled (\"tripped\") shunts.",
    'examples' => array(
      'drush shunt-get-enabled' => 'Get a list of enabled shunt trips.',
      'drush sge' => '',
    ),
    'aliases' => array('sge'),
  );

  $items['shunt-get-disabled'] = array(
    'description' => "Get a list of disabled shunt trips.",
    'examples' => array(
      'drush shunt-get-disabled' => 'Get a list of disabled shunt trips.',
      'drush sgd' => ''
    ),
    'aliases' => array('sgd'),
  );

  $items['shunt-is-enabled'] = array(
    'description' => "Determine if shunt is tripped/enabled.",
    'argument' => array(
      'name' => 'Optional. Name of the shunt to check (if you want to check one particular shunt).',
    ),
    'examples' => array(
      'drush shunt-is-enabled' => 'Determine if sandwich shunt is enabled',
      'drush shunt-is-enabled sandwich' => 'Determine if sandwich shunt is enabled',
      'drush sie sandwich' => ''
    ),
    'aliases' => array('sie', 'sit', 'shunt-is-tripped'),
  );

  return $items;
}

/**
 * drush command callback
 *
 * @param string $name
 *  Name of shunt to trip.
 * 
 * @see drush_invoke()
 * @see drush.api.php
 */
function drush_shunt_enable($name = NULL) {
  // Get/validate name of shunt to be tripped.
  $message = 'Enter a number to choose which shunt to trip/enable.';
  $shunts = _drush_shunt_name_choices();
  $choice = FALSE;

  // Check if the provided name ($name) is a valid shunt.
  if ($name && !array_key_exists($name, $shunts)) {
    return drush_set_error(dt("'!name' is not a valid shunt",
      array('!name' => $name)));
  }
  elseif ($name && $name !== 'all') {
    shunt_enable($name);
  }
  // If no $name is provided, offer choices.
  elseif (!$name) {
    $choice = drush_choice($shunts, $message, '!key');
  }

  // Trip shunt.
  if ($choice !== FALSE) {
    if ($choice == 'all' || $name == 'all') {
      $shunts = module_invoke_all('shunt');
      foreach ($shunts as $key => $description) {
        shunt_enable($key);
        drush_log(dt("'!key' shunt was tripped"));
      }
    }
    else {
      shunt_enable($choice);
      drush_log(dt("'!name' shunt was tripped"));
    }
  }
}

/**
 * Validate shunt name or prompt user with choices to select from. Return valid name.
 *
 * @param string $name
 *  Name of shunt to be enabled or disabled.
 *
 * @param string $message
 *  Message to display along with choices.
 *
 * @return string $name
 */
function _drush_shunt_name_choices() {
  $shunts = module_invoke_all('shunt');
  $shunts = array_merge(array('all' => 'all'), $shunts);
  return $shunts;
}

/**
 * drush command callback
 *
 * @param string $name
 *  Name of shunt to trip.
 *
 * @see drush_invoke()
 * @see drush.api.php
 */
function drush_shunt_disable($name = NULL) {
  // Get/validate name of shunt to be tripped.
  $message = 'Enter a number to choose which shunt to disable.';
  $shunts = _drush_shunt_name_choices();
  $choice = FALSE;

  // Check if the provided name ($name) is a valid shunt.
  if ($name && !array_key_exists($name, $shunts)) {
    return drush_set_error(dt("'!name' is not a valid shunt",
      array('!name' => $name)));
  }
  elseif ($name && $name !== 'all') {
    shunt_disable($name);
  }
  // If no $name is provided, offer choices.
  elseif (!$name) {
    $choice = drush_choice($shunts, $message, '!key');
  }

  // Trip shunt.
  if ($choice !== FALSE) {
    if ($choice == 'all' || $name == 'all') {
      $shunts = module_invoke_all('shunt');
      foreach ($shunts as $key => $description) {
        shunt_disable($key);
        drush_log(dt("'!key' shunt was disabled !insitename"));
      }
    }
    else {
      shunt_disable($choice);
      drush_log(dt("'!name' shunt was disabled !insitename"));
    }
  }
}

/**
 * drush command callback
 *
 * @see drush_invoke()
 * @see drush.api.php
 */
function drush_shunt_get_enabled(){
  $shunts = shunt_get_enabled();
  if (count($shunts) > 0) {
    $text = t('These shunts are currently "tripped" or enabled:');
    foreach ($shunts as $name) {
      $text .= "\n$name";
    }
  }
  else {
    $text = t('No shunts are currently "tripped" or enabled.');
  }

  drush_print($text);
}

/**
 * drush command callback
 */
function drush_shunt_get_disabled(){
  $shunts = shunt_get_disabled();
  if (count($shunts) > 0) {
    $text = t('These shunts are currently disabled:');
    foreach ($shunts as $name) {
      $text .= "\n$name";
    }
  }
  else {
    $text = t('No shunts are currently disabled. (All shunts are tripped.)');
  }

  drush_print($text);
}

/**
 * drush command callback
 */
function drush_shunt_is_enabled($name = '') {
  if ($name) {
    $shunt_is_enabled = shunt_is_enabled($name);   
    drush_print($name . 'shunt is tripped/enabled');
  }  
  else {
    $shunt_is_enabled = shunt_is_enabled();
    drush_print($name . 'shunt is NOT tripped/enabled');
  }
}
