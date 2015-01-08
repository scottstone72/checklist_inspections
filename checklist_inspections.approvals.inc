<?php

require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_inspections.class.inc.php');
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_category.class.inc.php');
drupal_add_css(drupal_get_path('module', 'checklist_inspections') . '/dist/css/min/check_inspect_approvals.min.css');
drupal_add_js(drupal_get_path('module', 'checklist_inspections') . '/dist/js/checklist_approval_form.min.js', array('scope' => 'footer'));

/**
 * Show form once callback has fired.
 * @return mixed
 */
function checklist_inspections_approval_page() {

  $build = array();
  $checklist = new ChecklistInspections(); // Get repair order object.
  $checklist_data = $checklist->get_checklist_approval_data();

  $build['explain_text'] = array(
    '#type' => 'markup',
    '#markup' => '<p>Admin page for editing and approving Checklist Inspection Submissions.
    Click the \'Show Details\' link to edit Checklist Inspection data and click \'Update\' to save changes.
    Add additional notes in the admin textarea if applicable.</p>',
  );

  $repair_data = array();

  foreach($checklist_data as $key => $val) {
    $build[] = drupal_get_form('checklist_inspections_approval_form_' . $val['order_num'], $val);

    // Unserialize to prepare for js.
    // + operator is a union for arrays, preserves array keys if there are no duplicates else will override.
    $repair_data = $repair_data + unserialize($val['repair_data']);
  }

  // Hand over to javascript.
  drupal_add_js(array('checklist_approval_data' => json_encode($repair_data)), 'setting');

  return $build;
}


/**
 * Implements hook_form().
 * Callback for Approvals Page
 */
function checklist_inspections_approval_form($form, &$form_state, $args) {

  global $user;

  $val = $args; // Pass in arguments.
  $repair_data = unserialize($args['repair_data']);

  drupal_add_library('system', 'ui.datepicker');

  // Create header array for table
  $header = array(
    'order_num' => t('Order #'),
    'unit_id' => t('Checklist Item'),
    'more_details' => t('Details'),
    'date_submitted' => t('Date'),
    'submitted_by' => t('Submitted by'),
  );

    // Vars for our form
    $order_num = $val['order_num'];
    $category = $val['category'];
    $unit_id = $val['unit_id'];
    $inspected_by = $val['inspected_by'];
    $date_submitted = $val['date_submitted'];
    $follow_up_alert = $val['follow_up_alert'];
    $comments = $val['comments'];
    $nid = $val['nid'];


  $links['pop_form'] = '<span value="'. $val['order_num'] .'" class="show-details">Show Details</span>';

    $rows = array();
    // Build the rows and add data for our table
    $rows[check_plain($val['order_num'])] = array(
      'order_num' => check_plain($val['order_num']),
      'unit_id' => check_plain($val['unit_id']),
      'more_details' => $links['pop_form'],
      'date_submitted'=> check_plain($val['date_submitted']),
      'submitted_by' => check_plain($val['inspected_by']),
    );

   $form['#attributes'] = array(
     'class' => array('checklist-inspections-approval-form'),
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
      //    '#id' => 'date-submitted',
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

    $form['container' . $order_num]['follow_up_alert'] = array(
      '#type' => 'textfield',
      '#title' => 'Follow Up Alert',
      '#default_value' => $follow_up_alert,
      '#id' => 'follow-up-' . $order_num,
      '#attributes' => array('class' => array('datepicker')),
      '#prefix' => '<div class="follow-up-alert" id="follow-up-alert-' . $order_num . '">',
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
      '#default_value' => '',
      '#rows' => 5,
      '#cols' => 100,
    );

    $form['container' . $order_num]['admin']['row_4_open']['#markup'] = '<div class="row">';


    $form['container' . $order_num]['admin']['admin_approver'] = array(
      '#type' => 'textfield',
      '#title' => t('Administrator name:'),
      '#value' => $user->name,
      '#prefix' => '<div class="admin-approver">',
      '#suffix' => '</div>',
      '#disabled' => TRUE,
    );



    $form['container' . $order_num]['admin']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Approve Repair'),
      '#id' => 'submit-' . $order_num,
      '#prefix' => '<div class="submit-button">',
      '#suffix' => '</div>',
    );

    $form['container' . $order_num]['admin']['cancel'] = array(
      '#markup' => '<div><span value="' . $order_num . '"  class="cancel-btn-val">Cancel</span></div>',
      '#prefix' => '<div class="cancel-button">',
      '#suffix' => '</div>',
    );

    $form['container' . $order_num]['admin']['row_4_closed']['#markup'] = '</div>';

  // Save these values for submission
  if(isset($category, $unit_id)) {
    $form['inspect_data'] = array(
      '#type' => 'value',
      '#value' => array(
        'category' => $category,
        'unit_id' => $unit_id,
        'nid' => $nid,
        'order_num' => $order_num,
      ),
    );
  }

  return $form;
}

function checklist_inspections_approval_form_validate($form, &$form_state) {

  $order_num = $form_state['values']['inspect_data']['order_num'];
  $repair_vals = array();
  $container = 'container' . $order_num; // Get the values in the container

  array_shift($form_state['input']); // pop container array off beginning of $form_state
  array_pop($form_state['input']); // pop approval array off end of $form_state

  foreach ($form_state['input'] as $key => $val) {

    // Filter $form_state values to just the values we need
    if (($key === 'op') || ($key === 'form_build_id') || ($key === 'form_token') || ($key === 'form_id'))
    {
      continue;
    }
    // Build our repair values array to store as a session
    // We need unique id's for every order for future pages,
    // so not to get duplicate id's on pages showing multiple forms.
    if (isset($val) && gettype($val) === 'string') {
      // If it is a radio button
      if (is_numeric($val)) {
        $repair_vals['order-'. $order_num . '-' . $key . '_' .$val] = $val;
      }
      // It is a checkbox button
      if ($val === 'on') {
        $repair_vals['order-'. $order_num . '-' . $key] = $val;
      }
    }
  }
  if (isset($form_state['values'][$container]['equip_state'])) {
    foreach ($form_state['values'][$container]['equip_state'] as $k => $v) {
      foreach ($v as $text) {
      $text_box['order-' . $order_num . '-' . $k] = $text;
      $repair_vals = $repair_vals + $text_box;
      }
    }
  }

  $order = array();
  $order[$order_num] = $repair_vals; // Need the order_num for javascript later on

  // Serialize the data to pass to submit function
  $store_repairs = serialize($order);
  $form_state['values']['repair_data'] = $store_repairs;
  $form_state['values']['repair_archive'] = $store_repairs;

}

function checklist_inspections_approval_form_submit($form, &$form_state) {

  global $user;
  $order_num = $form_state['values']['inspect_data']['order_num'];
  $container = 'container' . $order_num; // Get the values in the container

  $query = db_update('checklist_inspections');
  $fields = array(
    'order_num' => $form_state['values']['inspect_data']['order_num'],
    'date_submitted' => $form_state['values'][$container]['date_submitted'],
    'inspected_by' => $form_state['values'][$container]['inspected_by'],
    'follow_up_alert' => $form_state['values'][$container]['follow_up_alert'],
    'comments' => $form_state['values'][$container]['additional_notes']['comments'],
    'admin_notes' => $form_state['values'][$container]['admin']['notes'],
    'admin_approver' => $form_state['values'][$container]['admin']['admin_approver'],
    'category' => $form_state['values']['inspect_data']['category'],
    'unit_id' => $form_state['values']['inspect_data']['unit_id'],
    'nid' =>  $form_state['values']['inspect_data']['nid'],
    'repair_data' => $form_state['values']['repair_data'],
    'repair_archive' => $form_state['values']['repair_archive'],
    'approved' => 'approved',
  );
  $query->condition('order_num', $order_num)
  ->fields($fields);
  $query->execute();

  // Save session for print page
  $_SESSION['repair_data'] = $fields;

//  // Get emails that have been set in the transfer request configuration page.
//  $admin_mail = variable_get('transfer_admin_email');
//  // Send email to administrator notifying him of request.
//  $to = $admin_mail;
//  $from = $user->mail;
//
//  $params['order_num'] =  $form_state['values']['order_num'];
//  $params['date'] = $form_state['values']['date_submitted'];
//
//  $sent = drupal_mail('checklist_inspections', 'new_checklist', $to, language_default(), $params, $from, TRUE);
//  // Show error if mail does not get sent for whatever reason.
//  if(!$sent) {
//    drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
//  }


  drupal_set_message(t('Thank You! The Checklist Inspection has been submitted.'));

  $form_state['redirect'] = 'checklist_inspections/approvals/print';
}