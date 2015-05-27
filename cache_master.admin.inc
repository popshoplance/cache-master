<?php
/**
 * @file
 * Admin form for adding and editing cache paths.
 *
 */

/**
 * cache_master_form
 * Creates admin form.
 *
 * @return Form for administering module.
 */
function cache_master_form() {
  $form = array();
  $form['description'] = array(
    '#prefix' => '<div>',
    '#markup' => 'Set paths and expiration time in seconds.',
    '#suffix' => '</div>',
  );
  $form['cache_master_path'] = array(
      '#title' => t('Path'),
      '#type' => 'textfield',
      '#default_value' => variable_get('cache_master_path', '')
  );
  $form['cache_master_cache_time'] = array(
      '#title' => t('Cache Time'),
      '#type' => 'textfield',
      '#description' => t('Cache time in seconds for defined path.'),
      '#default_value' => variable_get('cache_master_cache_time', '')
  );

  $form['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Add path'),
  );

  $form['cache_master_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of active paths.'),
  );

  $cache_paths = cache_master_get_paths();

  if ($cache_paths) {
    foreach ($cache_paths as $key => $path) {
      $form['cache_master_tags']['meta_tag_' . $key] = array(
        '#prefix' => '<div>',
        '#markup' => check_plain($path['cache_master_path']) . ' <i>ttl: ' . check_plain($path['cache_master_time']) . '</i><div class="actions"><a href="/admin/config/system/cache-master/edit/' . $path['cache_master_uid'] . '">Edit</a> | <a href="/admin/config/system/cache-master/remove/' . $path['cache_master_uid'] . '">Remove</a>',
        '#suffix' => '</div>',
      );
    }
  }

  return $form;
}

/**
 * cache_master_edit_form
 * Creates editing form for tags.
 *
 * @return Form for editing tags.
 */
function cache_master_edit_form() {
  $form = array();
  $id = arg(5);
  $path = cache_master_get_paths($id);

  $form['cache_master_path'] = array(
      '#title' => t('Path'),
      '#type' => 'textfield',
      '#default_value' => $path['cache_master_path']
  );
  $form['cache_master_cache_time'] = array(
      '#title' => t('Cache Time'),
      '#type' => 'textfield',
      '#description' => t('Cache time in seconds for defined path'),
      '#default_value' => $path['cache_master_time']
  );

  $form['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Save path'),
  );

  $form['cancel'] = array(
   '#markup' => '<a href="/admin/config/system/cache-master">' . t('Cancel') . '</a>',
  );

  return $form;
}

/**
 * cache_master_form_validate
 * Form validation for cache_master_form.
 *
 * @param $form: cache_master_form.
 * @param &$form_state: Reference to state of cache_master_form.
 */
function cache_master_form_validate($form, &$form_state) {
  $path = $form_state['values']['cache_master_path'];
  $time = $form_state['values']['cache_master_cache_time'];
  if (empty($path)) {
    form_set_error('cache_master_path', t('Must provide a value for path.'));
  }
  if (empty($time)) {
    form_set_error('cache_master_cache_time', t('Must provide a value for ttl.'));
  }
  elseif (!is_numeric($time)) {
    form_set_error('cache_master_cache_time', t('ttl must be a number.'));
  }
}

/**
 * cache_master_form_submit
 * Form submission for cache_master_form.
 *
 * @param $form: cache_master_form.
 * @param &$form_state: Reference to state of cache_master_form.
 */
function cache_master_form_submit(&$form, &$form_state) {
  drupal_set_message(t('All your path belong to us. Path saved.'));

  cache_master_set_path($form_state['values']['cache_master_path'], $form_state['values']['cache_master_cache_time']);
}

/**
 * cache_master_edit_form_submit
 * Form submission for cache_master_edit_form.
 *
 * @param $form: cache_master_edit_form.
 * @param &$form_state: Reference to state of cache_master_edit_form.
 */
function cache_master_edit_form_submit(&$form, &$form_state) {
  db_update('cache_master')
    ->fields(array(
      'cache_master_path' => $form_state['input']['cache_master_path'],
      'cache_master_time' => $form_state['input']['cache_master_cache_time']
    ))
    -> condition('cache_master_uid', arg(5))
    ->execute();

  drupal_set_message(t('Path saved.'));
  drupal_goto('admin/config/system/cache-master');
}