<?php

class ChecklistInspections {

  // This call to database is for getting all the equipment categories.
  public function get_equipment_categories(){
    $categories = array();
//    $equip_categories = array();

    $query = db_select('field_data_field_unit_type_', 'f');
    $query->join('taxonomy_term_data', 't', 'f.field_unit_type__tid = t.tid');
    $query->groupBy('f.entity_id');
    $results = $query
      ->fields('t', array('name'))
      ->execute()->fetchCol();

    foreach($results as $value) {
      $categories[] = $value;
    }
    // We need to get rid of duplicate values in table column.
    $equip_categories = array_unique($categories);
    return $equip_categories;
  }

  // Populates the unit_id_select after user makes category selection.
  public function set_select_options_list($category){
    $unit_id_select = array();

    $query = db_select('node', 'n');
    $query->join('field_data_field_unit_type_', 'f', 'n.nid = f.entity_id');
    $query->groupBy('f.entity_id');
    $query->join('taxonomy_term_data', 't', 'f.field_unit_type__tid = t.tid');
    $query->groupBy('t.tid');
    $query->fields('t', array('name'));
    $query->fields('n', array('title','type', 'nid'));
    $query->fields('f', array('field_unit_type__tid'));
    $query->condition('t.name', $category);
    $results = $query->execute();

    foreach($results as $value) {
      $unit_id_select[$value->nid] = $value->title;
    }

    return $unit_id_select;
  }

  // Get the repair order number
  public function check_inspect_order_num() {

    $query = db_select('checklist_inspections', 'c');
    $query->addExpression('MAX(order_num)');
    $max= $query->execute()->fetchField();

    if(empty($max)) {
      $checklist_order_num = 1;
      return $checklist_order_num;
    }
    else {
      $checklist_order_num = $max;
      return $checklist_order_num + 1;
    }
  }


  // Get the data we need for the approval page
  public function get_checklist_approval_data() {

    $approval_data = array();

    $query = db_select('checklist_inspections', 'c')
      ->fields('c', array('order_num','category','unit_id','nid','approved','inspected_by','repair_data','date_submitted','follow_up_alert','comments'))
      ->condition('approved', NULL);
      $results = $query->execute();

    foreach ($results as $val) {
      $approval_data[] = (array) $val;
    }
    return $approval_data;
  }

  // Get the data we need for the checklist open page
  public function get_open_checklist_data($order = NULL) {

	$results = array();
    $open_inspect_data = array();
	if($order === NULL) {
	  $query = db_select('checklist_inspections', 'c')
		->fields('c', array('order_num', 'category', 'unit_id', 'nid', 'admin_notes', 'admin_approver',
		  'approved', 'inspected_by', 'repair_data', 'date_submitted', 'follow_up_alert', 'comments'))
		->condition('approved', 'approved')
		->condition('completed', NULL);
	  $results = $query->execute();
	} else {
	  $query = db_select('checklist_inspections', 'c')
		->fields('c', array('order_num', 'category', 'unit_id', 'nid', 'admin_notes', 'admin_approver',
		  'approved', 'inspected_by', 'repair_data', 'date_submitted', 'follow_up_alert', 'comments'))
		->condition('approved', 'approved')
		->condition('completed', NULL)
	  	->condition('order_num', $order);
	  $results = $query->execute();
	}

    foreach ($results as $val) {
      $open_inspect_data[] = (array) $val;
    }
    return $open_inspect_data;
  }

  // Get the data we need for the checklist archive page
  public function get_all_completed_checklist_data() {

    $completed_inspect_data = array();

      $query = db_select('checklist_inspections', 'c')
        ->fields('c', array('order_num','category','unit_id','nid', 'admin_notes', 'admin_approver',
          'approved','inspected_by','repair_archive','date_submitted','follow_up_alert','comments'))
        ->condition('approved', 'approved')
        ->condition('completed', 'completed');
        $results = $query->execute();

    foreach ($results as $val) {
      $completed_inspect_data[] = (array) $val;
    }
    return $completed_inspect_data;
  }

  // Get the data we need for the checklist archive page
  public function get_completed_checklist_data($unit_id) {

    $completed_inspect_data = array();

    $query = db_select('checklist_inspections', 'c')
      ->fields('c', array('order_num','category','unit_id','nid', 'admin_notes', 'admin_approver',
                          'approved','inspected_by','repair_archive','date_submitted','follow_up_alert','comments'))
      ->condition('approved', 'approved')
      ->condition('completed', 'completed')
      ->condition('unit_id', $unit_id);

    $results = $query->execute();

    foreach ($results as $val) {
      $completed_inspect_data[] = (array) $val;
    }
    return $completed_inspect_data;
  }

  // Get the data we need for the checklist archive Print page
  public function get_selected_completed_checklist_data($order = NULL) {
    $completed_inspect_data = array();

    $query = db_select('checklist_inspections', 'c')
      ->fields('c', array('order_num','category','unit_id','nid', 'admin_notes', 'admin_approver',
                          'approved','inspected_by','repair_archive','date_submitted','follow_up_alert','comments'))
      ->condition('approved', 'approved')
      ->condition('completed', 'completed')
      ->condition('order_num', $order);
    $results = $query->execute();

    foreach ($results as $val) {
      $completed_inspect_data[] = (array) $val;
    }
    return $completed_inspect_data;
  }


}

