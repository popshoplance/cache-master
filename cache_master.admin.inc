<?php
/**
 * @file
 * Admin form for adding and editing cache paths.
 *
 */

 /**
  * Displays the date format strings overview page.
  */
 function cache_master_admin() {
   $header = array(t('Path'), t('TTL'), array('data' => t('Operations'), 'colspan' => '2'));
   $rows = array();

   $paths = cache_master_get_paths();
   dpm($paths);
   if (!empty($paths)) {
     foreach ($paths as $path) {
       $row = array();
       $row[] = array('data' => $path['cache_master_path']);
       $row[] = array('data' => $path['cache_master_time']);
       $row[] = array('data' => l(t('edit'), 'admin/config/system/cache-master/edit/' . $path['id']));
       $row[] = array('data' => l(t('delete'), 'admin/config/system/cache-master/delete/' . $format['id']));
       $rows[] = $row;
     }
   }

   $build['date_formats_table'] = array(
     '#theme' => 'table',
     '#header' => $header,
     '#rows' => $rows,
     '#empty' => t('No Paths have been set. <a href="@link">Add Path</a>.', array('@link' => url('admin/config/system/cache-master/add'))),
   );

   return $build;
 }

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
  $time = $form_state['values']['cache_master_ttl'];
  if (empty($path)) {
    form_set_error('cache_master_path', t('Must provide a value for path.'));
  }
  if (empty($time)) {
    form_set_error('cache_master_ttl', t('Must provide a value for TTL.'));
  }
  elseif (!is_numeric($time)) {
    form_set_error('cache_master_ttl', t('TTL must be a number.'));
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

  cache_master_set_path($form_state['values']['cache_master_path'], $form_state['values']['cache_master_ttl']);
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
      'path' => $form_state['input']['cache_master_path'],
      'ttl' => $form_state['input']['cache_master_ttl']
    ))
    -> condition('cmid', arg(5))
    ->execute();

  drupal_set_message(t('Path saved.'));
  drupal_goto('admin/config/system/cache-master');
}
