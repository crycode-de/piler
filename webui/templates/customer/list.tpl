<div class="container text-danger text-start">
<?php if(isset($errorstring)) { print $text_error . ': ' . $errorstring; } ?>
</div>

<div class="container text-start">
  <div class="row">
    <div class="col">
    </div>
    <div class="col text-end">
      <form method="get" name="search1" action="customer.php">
        <div class="row justify-content-end">
          <div class="col-8">
            <input type="text" name="search" class="form-control" value="<?php print $search; ?>">
          </div>
          <div class="col-2">
            <button type="submit" class="btn btn-primary"><?php print $text_search; ?></button>
          </div>
        </div>
      </form>
    </div>
  </div>
</div>


<div class="container text-start">
  <div class="row">
    <div class="col">

      <h4><?php if(isset($id) && ($id > 0)) { print $text_edit_entry; } else { print $text_add_new_entry; } ?></h4>

      <form method="post" id="add1" name="add1" action="<?php print PATH_PREFIX; ?>index.php?route=customer/list" class="form-horizontal" enctype="multipart/form-data">

      <?php if(isset($id) && ($id > 0)) { ?>
        <input type="hidden" name="id" id="id" value="<?php print $id; ?>" />
      <?php } ?>

      <div class="row g-3 align-items-center">
        <div class="col-2">
          <label for="domain" class="col-form-label"><?php print $text_domain; ?></label>
        </div>
        <div class="col-5">
          <select name="domain" id="domain">
       <?php foreach ($domains as $domain) { ?>
          <option value="<?php print $domain; ?>"<?php if(isset($a['domain']) && $a['domain'] == $domain) { ?> selected="selected"<?php } ?>><?php print $domain; ?></option>
       <?php } ?>
          </select>
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      <div class="row g-3 align-items-center">
        <div class="col-2">
          <label for="branding_text" class="col-form-label"><?php print $text_branding_text; ?></label>
        </div>
        <div class="col-4">
          <input type="text" name="branding_text" id="branding_text" class="form-control" value="<?php if(isset($a['branding_text'])) { print $a['branding_text']; } ?>" aria-describedby="help1" />
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      <div class="row g-3 align-items-center">
        <div class="col-2">
          <label for="branding_url" class="col-form-label"><?php print $text_branding_url; ?></label>
        </div>
        <div class="col-4">
          <input type="text" name="branding_url" id="branding_url" class="form-control" value="<?php if(isset($a['branding_url'])) { print $a['branding_url']; } ?>" aria-describedby="help1" />
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      <div class="row g-3 align-items-center">
        <div class="col-2">
          <label for="support_link" class="col-form-label"><?php print $text_support_link; ?></label>
        </div>
        <div class="col-4">
          <input type="text" name="support_link" id="support_link" class="form-control" value="<?php if(isset($a['support_link'])) { print $a['support_link']; } ?>" aria-describedby="help1" />
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      <div class="row g-3 align-items-center">
        <div class="col-2">
          <label for="background_colour" class="col-form-label"><?php print $text_background_colour; ?></label>
        </div>
        <div class="col-4">
          <input type="text" name="background_colour" id="background_colour" class="form-control" placeholder="" value="<?php if(isset($a['background_colour'])) { print $a['background_colour']; } ?>" aria-describedby="help1" />
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      <div class="row g-3 align-items-center">
        <div class="col-2">
          <label for="text_colour" class="col-form-label"><?php print $text_text_colour; ?></label>
        </div>
        <div class="col-4">
          <input type="text" name="text_colour" id="text_colour" class="form-control" placeholder="" value="<?php if(isset($a['text_colour'])) { print $a['text_colour']; } ?>" aria-describedby="help1" />
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      <div class="row g-3 mt-2 align-items-center">
        <div class="col-2">
        </div>
        <div class="col-2">
          <button type="submit" class="btn btn-primary"><?php if(isset($id) && ($id > 0)) { print $text_modify; } else { print $text_add; } ?></button>
          <a href="<?php print PATH_PREFIX; ?>index.php?route=customer/list" type="reset" class="btn btn-secondary"><?php print $text_cancel; ?></a>
        </div>
        <div class="col-auto">
          <span id="help1" class="form-text"></span>
        </div>
      </div>

      </form>
    </div>
  </div>
</div>

<?php if($id == -1) { ?>

<div class="container text-start mt-5">
  <div class="row">
    <div class="col">

      <h4><?php print $text_existing_entries; ?></h4>

      <table class="table table-striped">
        <thead class="table-dark">
          <tr>
            <th><?php print $text_domain; ?></th>
            <th><?php print $text_branding_text; ?></th>
            <th><?php print $text_branding_url; ?></th>
            <th><?php print $text_branding_logo; ?></th>
            <th><?php print $text_background_colour; ?></th>
            <th><?php print $text_text_colour; ?></th>
            <th>&nbsp;</th>
            <th>&nbsp;</th>
          </tr>
        </thead>
        <tbody>
        <?php foreach($entries as $e) { ?>
          <tr>
            <td><?php print $e['domain']; ?></td>
            <td><?php print $e['branding_text']; ?></td>
            <td><?php print $e['branding_url']; ?></td>
            <td><?php if($e['branding_logo']) { ?><img src="/images/<?php print $e['branding_logo']; ?>" style="height: 50px;" /><?php } ?></td>
            <td><span class="label" style="background-color:<?php print $e['background_colour']; ?>"><?php print $e['background_colour']; ?></span></td>
            <td><span class="label" style="background-color:<?php print $e['text_colour']; ?>"><?php print $e['text_colour']; ?></span></td>
            <td><a href="<?php print PATH_PREFIX; ?>index.php?route=customer/list&amp;id=<?php print $e['id']; ?>"><i class="bi bi-pencil-square"></i>&nbsp;<?php print $text_edit; ?></a></td>
            <td><a href="<?php print PATH_PREFIX; ?>index.php?route=customer/remove&amp;id=<?php print $e['id']; ?>&amp;name=<?php print urlencode($e['domain']); ?>&amp;confirmed=1" class="confirm-delete" data-id="<?php print $e['id']; ?>" data-name="<?php print $e['domain']; ?>"><i class="bi bi-trash text-danger"></i>&nbsp;<?php print $text_remove; ?></a></td>
          </tr>
        <?php } ?>
        <tbody>
      </table>

    </div>
  </div>
</div>

<?php } ?>
