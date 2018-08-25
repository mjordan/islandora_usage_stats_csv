<?php

/**
 * @file
 * Block-related functions.
 */

/**
 * Implements hook_block_info().
 */
function islandora_usage_stats_csv_block_info() {
  $blocks = array();
  $blocks['object'] = array(
    'info' => t('Islandora Usage Stats CSV link'),
    'cache' => DRUPAL_CACHE_PER_USER,
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function islandora_usage_stats_csv_block_view($delta = '') {
  $block = array();
  $object = menu_get_object('islandora_object', 2);
  if ($delta == 'object' && $object) {
    module_load_include('inc', 'islandora_usage_stats_csv', 'includes/utilities');
    $allowed_cmodels = islandora_usage_stats_csv_check_for_cmodels($object);
    if ($delta == 'object') {
      if ($object && count($allowed_cmodels) > 0 && arg(3) != 'manage') {
        if (islandora_object_access('download islandora usage stats data', $object)) {
          $csv_link = l(t('Download CSV containing full usage stats'), '/islandora/object/' . $object->id . '/usage_stats_csv');
          $block['subject'] = t('Usage Statistics');
          $block['content'] = $csv_link;
        }
        return $block;
      }
    }
  }
}
