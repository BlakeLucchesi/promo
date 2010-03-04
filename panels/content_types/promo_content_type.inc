<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Promo'),
  'description' => t('Provides the ability to embed promo callouts into panel panes.'),

  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('promo_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'promo_content_type_render',
  // The default context.
  'defaults' => array(),

  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_no_context_content_type_edit_form.
  'edit form' => 'promo_content_type_edit_form',

  // Icon goes in the directory with the content type.
  'icon' => 'icon_example.png',
  'category' => array(t('Widgets'), -9),

  // this example does not provide 'admin info', which would populate the
  // panels builder page preview.
);


/**
 * 'Edit form' callback for the content type.
 * This example just returns a form; validation and submission are standard drupal
 * Note that if we had not provided an entry for this in hook_content_types,
 * this could have had the default name
 * ctools_plugin_example_no_context_content_type_edit_form.
 *
 */
function promo_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  if (!is_array($conf)) {
    $conf = array('pid' => '');
  }
  
  $query = db_query("SELECT pid, link, name, title, filepath FROM {promo} WHERE status = 1 ORDER BY title ASC");
  $options = array();
  while ($row = db_fetch_object($query)) {
    $options[$row->pid] = $row->title .' ('. $row->link. ')';
  }

  $form['pid'] = array(
    '#type' => 'select',
    '#title' => t('Select Promo Callout'),
    '#options' => $options,
    '#default_value' => $conf['pid']
  );

  return $form;
  return $form;
}

function promo_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function promo_content_type_render($subtype, $conf, $args, $context) {
  
  static $delta = 0;
  
  // Select the filepath for the promo
  $query = db_query("SELECT link, filepath, alt, title FROM {promo} WHERE pid = %d", $conf['pid']);
  $promo = db_fetch_object($query);

  $block = new stdClass();
  
  $block->module = 'promo';
  $block->delta = ++$delta;
  $block->subject = ''; //filter_xss_admin($conf['title']);
  $block->content = l(theme('image', $promo->filepath, $promo->alt, $promo->title), $promo->link, array(), null, null, false, true);

  return $block;

}