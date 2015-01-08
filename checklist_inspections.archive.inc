<?php

require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_inspections.class.inc.php');
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_category.class.inc.php');
drupal_add_css(drupal_get_path('module', 'checklist_inspections') . '/dist/css/min/check_inspect_open.min.css');
drupal_add_js(drupal_get_path('module', 'checklist_inspections') . '/dist/js/checklist_approval_form.min.js', array('scope' => 'footer'));

/**
 * Show form once callback has fired.
 * @return mixed
 */
function checklist_inspections_archive_page() {

  $build = array();
  $checklist = new ChecklistInspections(); // Get repair order object.
  $checklist_data = $checklist->get_completed_checklist_data();

  $build['explain_text'] = array(
	'#type' => 'markup',
	'#markup' => '<p>This page lists all Completed Checklist Inspection forms.
    Click the \'Show Details\' link to view details of each completed form.</p>',
  );

  $repair_data = array();

  foreach($checklist_data as $key => $val) {
	$build[] = drupal_get_form('checklist_inspections_archive_form_' . $val['order_num'], $val);

	// Unserialize to prepare for js.
	// + operator is a union for arrays, preserves array keys if there are no duplicates else will override.
	$repair_data = $repair_data + unserialize($val['repair_archive']);
  }

  // Hand over to javascript.
  drupal_add_js(array('checklist_approval_data' => json_encode($repair_data)), 'setting');

  return $build;
}


/**
 * Implements hook_form().
 * Callback for Approvals Page
 */
function checklist_inspections_archive_form($form, &$form_state, $args) {

  global $user;

  $val = $args; // Pass in arguments.
  $repair_data = unserialize($args['repair_archive']);

  // Create header array for table
  $header = array(
	'order_num' => t('Order #'),
	'unit_id' => t('Checklist Item'),
	'more_details' => t('Details'),
	'date_submitted' => t('Date'),
	'submitted_by' => t('Submitted by'),
	'print' => t('Print'),
  );

  // Vars for our form
  $order_num = $val['order_num'];
  $category = $val['category'];
  $unit_id = $val['unit_id'];
  $inspected_by = $val['inspected_by'];
  $date_submitted = $val['date_submitted'];
//  $follow_up_alert = $val['follow_up_alert'];
  $comments = $val['comments'];
//  $nid = $val['nid'];
  $admin_notes = $val['admin_notes'];
  $admin_approver = $val['admin_approver'];

  $links['pop_form'] = '<span value="'. $val['order_num'] .'" class="show-details">Show Details</span>';
  $links['print'] = '<a href="/checklist_inspections/archive/print/'. $order_num .'">Print Order</a>';

  $rows = array();
  // Build the rows and add data for our table
  $rows[check_plain($val['order_num'])] = array(
	'order_num' => check_plain($val['order_num']),
	'unit_id' => check_plain($val['unit_id']),
	'more_details' => $links['pop_form'],
	'date_submitted'=> check_plain($val['date_submitted']),
	'submitted_by' => check_plain($val['inspected_by']),
	'print' => $links['print'],
  );

  $form['#attributes'] = array(
	'class' => array('checklist-inspections-open-form'),
  );

  // Construct the table
  $form['approval_table' . $order_num] = array(
	'#type' => 'tableselect',
	'#header' => $header,
	'#options' => $rows,
	'#js_select' => FALSE,
	'#empty' => t('No users found'),
	'#multiple' => TRUE,
  );

  // Build drop down fieldset to hold input elements
  $form['container' . $order_num] = array(
	'#type' => 'fieldset',
	'#tree' => TRUE,
	'#attributes' => array('class' => array('approval-fieldset')),
  );

//    $form['container'] = array(
//      '#type' => 'fieldset',
//    );

  $form['container' . $order_num]['category'] = array(
	'#type' => 'markup',
	'#markup' => '<div class="category" id="category-' . $order_num . '"><h3>' . $category . '</h3></div>',
  );

  $form['container' . $order_num]['unit_id'] = array(
	'#type' => 'markup',
	'#markup' => '<div class="unit-id" id="unit-id-' . $order_num . '"><h4>' . $unit_id . '</h4></div>',
  );


  $form['container' . $order_num]['row_1_open']['#markup'] = '<div class="row">';

  $form['container' . $order_num]['order_num'] = array(
	'#type' => 'textfield',
	'#title' => 'Order #',
	'#value' => $order_num,
	'#disabled' => TRUE,
	'#prefix' => '<div class="order-num" id="order-num' .$order_num . '">',
	'#suffix' => '</div>',
  );

  $form['container' . $order_num]['date_submitted'] = array(
	'#type' => 'textfield',
	'#title' => 'Date Submitted',
	'#value' => $date_submitted,
	'#size' => 25,
	'#disabled' => TRUE,
	'#prefix' => '<div class="date-submitted" id="date-submitted' . $order_num . '">',
	'#suffix' => '</div>',
  );

  $form['container' . $order_num]['inspected_by'] = array(
	'#type' => 'textfield',
	'#title' => t('Inspected By'),
	'#value' => $inspected_by,
	'#disabled' => TRUE,
	'#prefix' => '<div class="inspected-by" id="inspected-by' . $order_num . '">',
	'#suffix' => '</div>',
  );


  $form['container' . $order_num]['row_1_closed']['#markup'] = '</div><div class="row">'; // Close the first row, Open second row


  $form['container' . $order_num]['checklist_message']['#markup'] = '<div class="checklist-message" id="checklist-message-' .
	$order_num . '"><p><strong>S</strong> = <em class="satisfactory-info" id="satisfactory-info' . $order_num .
	'">Satisfactory</em>&nbsp<strong>D</strong> = <em class="deficient-info" id="deficient-info' .
	$order_num . '">Deficient</em>&nbsp <strong>N/A</strong> = <em class="NA-info" id="NA-info' . $order_num . '"> Not Applicable</em></p></div>';

  $form['container' . $order_num]['row_2_closed']['#markup'] = '</div>'; // Close second row

  $form['container' . $order_num]['equip_state'] = array(
	'#type' => 'fieldset',
	'#title' => t('State of Repair'),
	'#id' => 'edit-equip-state-' . $order_num,
	'#attributes' => array(
	  'class' => array('edit-equip-state'),
	),
  );

  if ($category && $unit_id) {

	$category_obj = new ChecklistCategories();
	$category_list = $category_obj->build_category_list($category);
	$repair_rows = array();
	$repair_states = array();

	foreach ($category_list as $box => $rad) {
	  // Reorganize into the array format we need
	  if ($box === 'title') {
		foreach ($rad as $y => $x) {
		  $repair_rows[$y] = $x;
		}
	  }
	  if ($box === 'repair_state') {
		foreach ($rad as $b => $c) {
		  $repair_states[$b] = $c;
		}
	  }
	}

	// Start building the all the checkboxes inside each fieldset.
	foreach ($repair_rows as $key => $state) {
	  $form['container' . $order_num]['equip_state'][$key] = array(
		'#type' => 'fieldset',
		'#attributes' => array('class' => array('ratings-container')),
		'#tree' => TRUE,
	  );


	  //Build the checkbox states
	  $form['container' . $order_num]['equip_state'][$key][$state] = array(
		'#type' => 'markup',
		'#markup' => t('<div class="status-titles pull-left"><strong>@state</strong></div>', array('@state' => $state)),
		//        '#attributes' => array(
		//          'class' => array($key),
		//        ),
	  );


	  foreach ($repair_states as $key_val => $cond) {
		// Build our radio buttons
		$form['container' . $order_num]['equip_state'][$key][$key . '-' . $cond] = array(
		  '#type' => 'markup',
		  '#markup' => '<div class="btn-group btn-group-sm pull-right radio-buttons" data-toggle="buttons">
                          <label for="order-'. $order_num . '-' . $key . '_radio_1" class="btn btn-primary radios">
                            <input type="radio" name="' . $key . '_radio" id="order-'. $order_num . '-' . $key . '_radio_1" value="1">S
                          </label>
                          <label for="order-'. $order_num . '-' . $key . '_radio_2" class="btn btn-primary radios">
                            <input type="radio" name="' . $key . '_radio" id="order-'. $order_num . '-' . $key . '_radio_2" value="2">D
                          </label>
                          <label for="order-'. $order_num . '-' . $key . '_radio_3" class="btn btn-primary radios">
                            <input type="radio" name="' . $key . '_radio" id="order-'. $order_num . '-' . $key . '_radio_3" value="3">N/A
                          </label>
                        </div>'
		);

		// Compare key value in $repairs_state array with key value in $rows titles
		// being cycled in the outside foreach loop.
		if ($key_val == $key) {

		  $form['container' . $order_num]['equip_state'][$key]['checkbox_group'] = array(
			'#prefix' => '<div class="btn-group btn-group-sm pull-right damage-buttons" data-toggle="buttons">',
			'#suffix' => '</div>',
		  );
		  // If there is a match than print checkboxes for each repair state value
		  foreach ($cond as $val) {

			$form['container' . $order_num]['equip_state'][$key]['checkbox_group'][$val] = array(
			  '#type' => 'markup',
			  '#markup' => '<label for="order-'. $order_num . '-' . $key . '_' . $val . '" class="btn btn-default checkboxes">
                                <input type="checkbox"  name="' . $key . '_' . $val . '" id="order-'. $order_num . '-' . $key . '_' . $val . '">' . $val . '</label>'
			);
		  }
		}
	  }

	  $is_true = FALSE;
	  switch ($key) {
		case 'wiring':
		  $is_true = TRUE;
		  break;
		case 'dry_amps':
		  $is_true = TRUE;
		  break;
		case 'pumping_amps':
		  $is_true = TRUE;
		  break;
		case 'pressure':
		  $is_true = TRUE;
		  break;
		case 'propane_level':
		  $is_true = TRUE;
		  break;
		case 'certification':
		  $is_true = TRUE;
		  break;
		case 'tire_pressure_tires':
		  $is_true = TRUE;
		  break;
		case 'engine_hours':
		  $is_true = TRUE;
		  break;
		case 'oil_level':
		  $is_true = TRUE;
		  break;
		case 'fuel_level':
		  $is_true = TRUE;
		  break;
		case 'trans_and_trans_case':
		  $is_true = TRUE;
		  break;
		case 'differentials':
		  $is_true = TRUE;
		  break;
		case 'hydraulic_tank':
		  $is_true = TRUE;
		  break;
		case 'fuel_tank':
		  $is_true = TRUE;
		  break;
		case 'engine_coolant':
		  $is_true = TRUE;
		  break;
		case 'fire_extinguisher':
		  $is_true = TRUE;
		  break;
		case 'window_wipers_washer':
		  $is_true = TRUE;
		  break;
		default;
	  }

	  // Only print textfields that apply
	  if ($is_true) {
		foreach ($repair_data as $x => $y) {
		  foreach($y as $data_key => $data_val) {
			$data = 'order-'. $order_num . '-' . $key; // Need a match to print textbox value
			// Filter status till we get textbox value
			if($data === $data_key) {

			  $form['container' . $order_num]['equip_state'][$key][$key . '_textbox'] = array(
				'#type' => 'textfield',
				'#default_value' => $data_val,
				'#size' => 60,
				'#disabled' => TRUE,
				'#id' => 'order-' . $order_num . '-' . $key,
				'#attributes' => array('class' => array('text-box')),
				//            '#disabled' => TRUE,
			  );
			}
		  }
		}
	  }
	}
  }

  $form['container' . $order_num]['additional_notes'] = array(
	'#type' => 'fieldset',
	'#title' => t('Additional Notes'),
  );

  $form['container' . $order_num]['additional_notes']['comments'] = array(
	'#type' => 'textarea',
	'#title' => t('Comments'),
	'#default_value' => $comments,
	'#id' => 'order-' . $order_num . '-comments',
	'#disabled' => TRUE,
  );


  // This is the admin section
  $form['container' . $order_num]['admin'] = array(
	'#type' => 'fieldset',
	'#title' => 'Admin Section',
  );

  $form['container' . $order_num]['admin']['notes'] = array(
	'#type' => 'textarea',
	'#title' => t('Notes:'),
	'#default_value' => $admin_notes,
	'#disabled' => TRUE,
	'#rows' => 5,
	'#cols' => 100,
  );

  $form['container' . $order_num]['admin']['row_4_open']['#markup'] = '<div class="row">';


  $form['container' . $order_num]['admin']['admin_approver'] = array(
	'#type' => 'textfield',
	'#title' => t('Administrator name:'),
	'#value' => $admin_approver,
	'#prefix' => '<div class="admin-approver">',
	'#suffix' => '</div>',
	'#disabled' => TRUE,
  );


  $form['container' . $order_num]['admin']['cancel'] = array(
	'#markup' => '<div><span value="' . $order_num . '"  class="cancel-btn-val">Cancel</span></div>',
	'#prefix' => '<div class="cancel-button">',
	'#suffix' => '</div>',
  );

  $form['container' . $order_num]['admin']['row_4_closed']['#markup'] = '</div>';


  return $form;
}

