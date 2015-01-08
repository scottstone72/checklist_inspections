<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_inspections.class.inc.php');
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_category.class.inc.php');
drupal_add_css(drupal_get_path('module', 'checklist_inspections') . '/dist/css/min/check_inspect_print.min.css');
drupal_add_js(drupal_get_path('module', 'checklist_inspections') . '/dist/js/checklist_submit_print.min.js', array('scope' => 'footer'));

/**
 * Page callback for printing submitted Checklist Inspection.
 */
function checklist_open_callback_print($args) {

  $repair_data = array();
  $checklist = new ChecklistInspections(); // Get repair order object.
  $checklist_data = $checklist->get_open_checklist_data($args);

  foreach ($checklist_data as $value) {

	// unpack our session data
	$unit_id = $value['unit_id'];
	$category = $value['category'];
	$date_submitted = $value['date_submitted'];
	$order_num = $value['order_num'];
	$inspected_by = $value['inspected_by'];
	$comments = $value['comments'];
	$follow_up_alert = $value['follow_up_alert'];
	$admin_notes = $value['admin_notes'];
	$admin_approver = $value['admin_approver'];

	$repair_data = unserialize($value['repair_data']);

	// Send image through the theme_image function.
	$image_path = drupal_get_path('theme', 'ponderosa_bootstrap') . '/images/ponderosa_logo.png';
	$variables = array(
	  'path' => $image_path,
	  'alt' => 'Company Logo',
	  'title' => 'Company Logo',
	  'attributes' => array('class' => 'company-image', 'id' => 'company-image'),
	);
	$img = theme('image', $variables);

	global $user;
	$output = array();

	$output['container'] = array(
	  '#type' => 'markup',
	  '#prefix' => '<div>' . $img . '</div>',
	);

	$output['container']['checklist_title'] = array(
	  '#type' => 'markup',
	  '#prefix' => '<div id="checklist-title"><h2>Open Checklist Inspections</h2></div>',
	);

	$output['container']['category'] = array(
	  '#type' => 'markup',
	  '#markup' => '<div id="category"><h3><strong>' . $category . '</strong></h3></div>',
	);

	$output['container']['unit_id_select'] = array(
	  '#type' => 'markup',
	  '#markup' => '<div id="unit_id"><h4><strong>' . $unit_id . '</strong></h4></div>',
	);

	$output['container']['order_num'] = array(
	  '#type' => 'markup',
	  '#markup' => '<div id="order-num"><strong>Order #: &nbsp</strong>' . $order_num . '</div>',
	);

	$output['container']['date_submitted'] = array(
	  '#type' => 'markup',
	  '#markup' => '<div><strong>Date: &nbsp</strong>' . $date_submitted . '</div>',
	  '#prefix' => '<div id="date-submitted">',
	  '#suffix' => '</div>',
	);


	$output['container']['inspected_by'] = array(
	  '#type' => 'markup',
	  '#markup' => '<div><strong>Inspected By: &nbsp</strong>' . $inspected_by . '</div>',
	  '#prefix' => '<div id="inspected-by">',
	  '#suffix' => '</div>',
	);


	$output['container']['checklist_message'] = array(
	  '#markup' => '<div id="user-instructions"><strong>S</strong> = <em>Satisfactory</em>&nbsp
        <strong>D</strong> = <em>Deficient</em>&nbsp <strong>N/A</strong> = <em> Not Applicable</em></div>'
	);

	// Fieldset to contain each row of information
	$output['container']['equip_state'] = array(
	  '#type' => 'markup',
	  '#markup' => '<div id="edit-equip-state">State of Repair</div>',
	  //    '#id' => t('edit-equip-state'),
	);

	$category_obj = new ChecklistCategories();
	$category_list = $category_obj->build_category_list($category);
	$rows = array();
	$repair_states = array();

	foreach ($category_list as $box => $rad) {
	  // Reorganize into the array format we need
	  if ($box === 'title') {
		foreach ($rad as $y => $x) {
		  $rows[$y] = $x;
		}
	  }
	  if ($box === 'repair_state') {
		foreach ($rad as $b => $c) {
		  $repair_states[$b] = $c;
		}
	  }
	}


	// Start building the all the checkboxes inside each fieldset.
	foreach ($rows as $key => $state) {
	  $output['container'][$key] = array(
		'#type' => 'fieldset',
		'#attributes' => array('class' => array('ratings-container')),
		'#tree' => TRUE,
	  );

	  // Add the repair states for each row if it is a string value
	  if (gettype($state) === 'string') {
		$output['container'][$key][$state] = array(
		  '#type' => 'markup',
		  '#markup' => t("<strong class='repair-title'>@state</strong>", array('@state' => $state)),
		  '#attributes' => array(
			'id' => array($key),
			//          'class' => array('col-sm-4'),
		  ),
		);
	  }


	  foreach ($repair_states as $key_val => $cond) {
		// Build our radio buttons
		$output['container'][$key]['radios'] = array(
		  '#type' => 'markup',
		  '#markup' => '<div class="radio-buttons-right">
                          <div class="repair-radio-buttons"><span id="order-' . $order_num . '-' . $key . '_radio_1" value="1"><strong>S</strong></span></div>
                          <div class="repair-radio-buttons"><span id="order-' . $order_num . '-' . $key . '_radio_2" value="2"><strong>D</strong></span></div>
                          <div class="repair-radio-buttons"><span id="order-' . $order_num . '-' . $key . '_radio_3" value="3"><strong>N/A</strong></span></div>
                      </div>'
		);

		// Compare key value in $repairs_state array with key value in $rows titles
		// being cycled in the outside foreach loop.
		if ($key_val == $key) {

		  $output['container'][$key]['checkbox_group'] = array(
			'#prefix' => '<div class="checkbox-group">',
			'#suffix' => '</div>',
		  );

		  // If there is a match than print checkboxes for each repair state value
		  foreach ($cond as $val) {
			$output['container'][$key]['checkbox_group'][$val] = array(
			  '#type' => 'markup',
			  '#markup' => '<div  class="checkbox-values">
                              <span name="' . $key . '_' . $val . '" id="order-' . $order_num . '-' . $key . '_' . $val . '">' . $val . '</span></div>'
			);
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

		// Only print textfields that apply along with the value
		if ($is_true) {
		  foreach ($repair_data as $y) {
			foreach ($y as $data_key => $data_val) {

			  $data = 'order-' . $order_num . '-' . $key; // Need a match to print textbox value

			  // Filter status till we get textbox value
			  if ($data === $data_key) {
				$output['container'][$key][$key . '_textbox'] = array(
				  '#type' => 'markup',
				  '#markup' => '<div class="status-comments">: <span class="stat-text">' . $data_val . '</span></div>'
				);
			  }
			}
		  }
		}
	  }
	}


	$output['container']['comments'] = array(
	  '#type' => 'markup',
	  '#markup' => t('<div id="comments"><p style="display: inline-block">Comments: &nbsp @comments</p></div>', array('@comments' => $comments)),
	);


	$output['container']['follow_up_alert'] = array(
	  '#markup' => t('<div id="follow-up-alert"><p style="display: inline-block">Follow Up By: &nbsp @follow-up-alert</p></div>', array('@follow-up-alert' => $follow_up_alert)),
	);

	$output['container']['admin_notes'] = array(
	  '#type' => 'markup',
	  '#markup' => t('<div id="admin-notes"><p style="display: inline-block">Admin Notes: &nbsp @admin_notes</p></div>', array('@admin_notes' => $admin_notes)),
	);

	$output['container']['admin_approver'] = array(
	  '#type' => 'markup',
	  '#markup' => t('<div id="admin-approver"><p style="display: inline-block">Admin Approver: &nbsp @admin_approver</p></div>', array('@admin_approver' => $admin_approver)),
	);
  }

  $output['container']['checklist_return_link'] = array(
	'#type' => 'markup',
	'#markup' => '<div id="link-container"><ul class="link-list"><li>' . l(t(' <- back to Open Checklist Inspection Forms'), 'checklist_inspections/open_inspections',
		array('attributes' => array('class' => array('transfer-form-link')))) . "</li>",
  );

  $output['container']['home_return_link'] = array(
	'#type' => 'markup',
	'#markup' => '<li>' . l(t('Home'), '', array('attributes' => array('class' => array('home-link')))) . "</li>",
  );

  $output['container']['print_link'] = array(
	'#type' => 'markup',
	'#markup' => '<li id="print-link"><div id="print-this-page">Print Page</div></li></ul></div>',
  );

  drupal_add_js(array('checklist_inspections' => $repair_data), 'setting');

  return $output;
}