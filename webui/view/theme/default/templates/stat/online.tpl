
<h4><?php print $text_online_users; ?></h4>

<div class="listarea">

<?php if(isset($users)){ ?>

   <table id="ss1" class="table table-striped table-condensed">
      <tr>
         <th class="domaincell"><?php print $text_username; ?></th>
         <th class="domaincell"><?php print $text_ipaddr; ?></th>
         <th class="domaincell"><?php print $text_logged_in; ?></th>
         <th class="domaincell"><?php print $text_last_activity; ?></th>
      </tr>

<?php foreach($users as $user) { ?>
      <tr>
         <td class="domaincell"><?php print $user['username']; ?></td>
         <td class="domaincell"><?php print $user['ipaddr']; ?></td>
         <td class="domaincell"><?php print date("Y.m.d. G:i:s", $user['ts']); ?></td>
         <td class="domaincell"><?php print date("Y.m.d. G:i:s", $user['last_activity']); ?></td>
      </tr>
<?php } ?>

   </table>

<?php } ?>

</div>

