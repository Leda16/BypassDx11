<?php
session_start();
?>


                            <?php
                    if(isset($_SESSION['noauthenticado'])):
                    ?>
                  
                    <?php
                    endif;
                    unset($_SESSION['noauthenticado']);
                    ?>

          



<!DOCTYPE html>

<!-- beautify ignore:start -->
<html lang="en" class="light-style  customizer-hide" dir="ltr" data-theme="theme-default" data-assets-path="../assets/" data-template="vertical-menu-template-free">

  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0">

    <title>LA Bypass | Register</title>
    
    <meta name="description" content="Most Powerful &amp; CSGO cheat">
    <meta name="keywords" content="dashboard, bootstrap 5 dashboard, bootstrap 5 design, bootstrap 5, csgo, midnight, neverlose, aimware, csgo, fivem, fivem bypass, bypass">

    <!-- Favicon -->
    <link rel="icon" type="image/x-icon" href="favicon.ico">

    <!-- Fonts -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="css/css2.css" rel="stylesheet">

    <!-- Icons. Uncomment required icon fonts -->
    <link rel="stylesheet" href="../css/boxicons.css">
    
    

    <!-- Core CSS -->
    <link rel="stylesheet" href="../css/core.css" class="template-customizer-core-css">
    <link rel="stylesheet" href="../css/theme-default.css" class="template-customizer-theme-css">
    <link rel="stylesheet" href="../css/demo.css">

    <!-- Vendors CSS -->
    <link rel="stylesheet" href="../css/perfect-scrollbar.css">
    
    

    <!-- Page CSS -->
    <!-- Page -->
<link rel="stylesheet" href="../css/page-auth.css">
    <!-- Helpers -->
    <script src="js/helpers.js"></script>

    <!--! Template customizer & Theme config files MUST be included after core stylesheets and helpers.js in the <head> section -->
    <!--? Config:  Mandatory theme config file contain global vars & default theme options, Set your preferred theme option in this file.  -->
    <script src="../js/config.js"></script>
    
    <!-- Global site tag (gtag.js) - Google Analytics -->
    <script async="async" src="js_1"></script>
    <script>
    window.dataLayer = window.dataLayer || [];

    function gtag() {
      dataLayer.push(arguments);
    }
    gtag('js', new Date());
    gtag('config', 'GA_MEASUREMENT_ID');
    </script>
    <!-- Custom notification for demo -->
    <!-- beautify ignore:end -->

</head>

<body>

  <!-- Content -->

<div class="container-xxl">
  <div class="authentication-wrapper authentication-basic container-p-y">
    <div class="authentication-inner">
      <!-- Register -->
      <div class="card">
        <div class="card-body">
          <!-- Logo -->
          <div class="app-brand justify-content-center">
            <a href="index.html" class="app-brand-link gap-2">
              <span class="app-brand-logo demo">
            <?php
                    if(isset($_SESSION['noauthenticado'])):
                    ?>
                                    <?php
                    endif;
                    unset($_SESSION['noauthenticado']);
                    ?>

</span>
            </a>
          </div>
          <!-- /Logo -->
          <h4 class="mb-2">Welcome to LA Bypass! 👋</h4>
          <p class="mb-4">Please sign-in to your account and start the adventure</p>

   <?php
                    if(isset($_SESSION['noauthenticado'])):
                    ?>
                    <?php
                    endif;
                    unset($_SESSION['noauthenticado']);
                    ?>

   
          
                            <form class="mx-auto mb-1" style="max-width:400px" action="logincheck.php" method="POST">

                                <div class="mb-3">
                                    <label for="usuario" class="form-label">Key</label>
                                    <input class="form-control" name="usuario" type="username" id="username" required="" placeholder="Enter your key">
                                </div>


                    

                                <div class="mb-3 mb-0 text-center">
              <button class="btn btn-primary d-grid w-100" type="submit">Sign in</button>
                                </div>

                            </form>
        </div>
      </div>
      <!-- /Register -->
    </div>
  </div>
</div>

<!-- / Content -->

  


  

  <!-- Core JS -->
  <!-- build:js assets/vendor/js/core.js -->
  <script src="../js/jquery.js"></script>
  <script src="../js/popper.js"></script>
  <script src="../js/bootstrap.js"></script>
  <script src="../js/perfect-scrollbar.js"></script>
  
  <script src="../js/menu.js"></script>
  <!-- endbuild -->

  <!-- Vendors JS -->
  
  

  <!-- Main JS -->
  <script src="../js/main.js"></script>

  <!-- Page JS -->
  
  
  
  <!-- Place this tag in your head or just before your close body tag. -->
  <script async defer src="../js/buttons.js"></script>
  
</body>

</html>
