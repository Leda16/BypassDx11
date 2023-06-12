<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

ini_set('display_errors', 0);

session_start();
if(!$_SESSION['usuario']) {
	header('Location: ../');
	exit();
}
include('../conexao.php');


$user_id = $_SESSION['usuario'];


 
 
 
	$email_usuario = $_POST['txt_imageuser'];


if(isset($_POST['update'])){

   
       //read a file from source
    $src_image =   file_get_contents("$email_usuario");

    //give new file name
    $new_info = "./photos/$user_id.png";

    //write data to a file
    $status = file_put_contents($new_info, $src_image);
   
   
}

?> 



<!DOCTYPE html>


<!-- beautify ignore:start -->
<html lang="en" class="light-style layout-menu-fixed " dir="ltr" data-theme="theme-default" data-assets-path="../assets/" data-template="vertical-menu-template-free">

  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0">

    <title>LA Bypass | Dashboard</title>
    
    <meta name="description" content="Most Powerful Loader">
    <meta name="keywords" content="dashboard, bootstrap 5 dashboard, bootstrap 5 design, bootstrap 5, csgo, midnight, neverlose, aimware, aimtux, la bypass, bypass, fivem bypass, fivem">

    <!-- Favicon -->
    <link rel="icon" type="image/x-icon" href="../../logo.ico">

    <!-- Fonts --> 
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="../css/css2.css" rel="stylesheet">

    <!-- Icons. Uncomment required icon fonts -->
    <link rel="stylesheet" href="../css/boxicons.css">
    
    

    <!-- Core CSS -->
    <link rel="stylesheet" href="../css/core.css" class="template-customizer-core-css">
    <link rel="stylesheet" href="../css/theme-default.css" class="template-customizer-theme-css">
    <link rel="stylesheet" href="../css/demo.css">

    <!-- Vendors CSS -->
    <link rel="stylesheet" href="../css/perfect-scrollbar.css">
    
    <link rel="stylesheet" href="../css/apex-charts.css">

    <!-- Page CSS -->
    
    <!-- Helpers -->
    <script src="../js/helpers.js"></script>

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

<body style="background-color:rgb(14,14,14);" >

  <!-- Layout wrapper -->
<div class="layout-wrapper layout-content-navbar  ">
  <div class="layout-container">

    
    




<!-- Menu -->

<aside id="layout-menu" class="layout-menu menu-vertical menu bg-menu-theme">

  
  <div class="app-brand demo ">
    <a href="index.html" class="app-brand-link">
       <span class="app-brand-logo demo">


        <h3> LA Loader</h3>

</span>
    </a>

    <a href="javascript:void(0);" class="layout-menu-toggle menu-link text-large ms-auto d-block d-xl-none">
      <i class="bx bx-chevron-left bx-sm align-middle"></i>
    </a>
  </div>

  <div class="menu-inner-shadow"></div>

  

  <ul class="menu-inner py-1">

    <!-- Dashboard -->
    <li style="before::background:rgb(0,216,9);" class="menu-item active">
      <a href="index.html" class="menu-link">
        <i class="menu-icon tf-icons bx bx-home-circle"></i>
        <div data-i18n="Analytics">Dashboard</div>
      </a>
    </li>

    <!-- Layouts -->
   
    <li class="menu-header small text-uppercase">
      <span class="menu-header-text">Downloads</span>
    </li>
    <li class="menu-item">
      <a href="https://cdn.discordapp.com/attachments/1008005803262881903/1011796975127113830/Optimizer-13.6.exe" target="_blank" class="menu-link">
        <i class="menu-icon tf-icons bx bx-download"></i>
        <div data-i18n="Support">Download</div>
      </a>
    </li>

   
    
</aside>
<!-- / Menu -->

    

    <!-- Layout container -->
    <div class="layout-page">
      
      



<!-- Navbar -->




<nav class="layout-navbar container-xxl navbar navbar-expand-xl navbar-detached align-items-center bg-navbar-theme" id="layout-navbar">
  

  

  

      
      

      
      
      <div class="layout-menu-toggle navbar-nav align-items-xl-center me-3 me-xl-0   d-xl-none ">
        <a class="nav-item nav-link px-0 me-xl-4" href="javascript:void(0)">
          <i class="bx bx-menu bx-sm"></i>
        </a>
      </div>
      

      <div class="navbar-nav-right d-flex align-items-center" id="navbar-collapse">

        



        <ul class="navbar-nav flex-row align-items-center ms-auto">
          

          
   

          <!-- User -->
          <li class="nav-item navbar-dropdown dropdown-user dropdown">
            <a class="nav-link dropdown-toggle hide-arrow" href="javascript:void(0);" data-bs-toggle="dropdown">
              <div class="avatar avatar-online">
                <img src="photos/<?php echo $_SESSION['usuario'];?>.png" alt class="w-px-40 h-auto rounded-circle">
              </div>
            </a>
            <ul class="dropdown-menu dropdown-menu-end">
              <li>
                <a class="dropdown-item" href="#">
                  <div class="d-flex">
                    <div class="flex-shrink-0 me-3">
                      <div class="avatar avatar-online">
                        <img src="photos/<?php echo $_SESSION['usuario'];?>.png" alt class="w-px-40 h-auto rounded-circle">
                      </div>
                    </div>
                    <div class="flex-grow-1">
                      <span class="fw-semibold d-block"><?php echo $_SESSION['usuario'];?></span>
                      <small class="text-muted">Customer</small>
                    </div>
                  </div>
                </a>
              </li>
              <li>
                <div class="dropdown-divider"></div>
              </li>
              <li>
            
              
              <li>
                <a class="dropdown-item" href="https://labypass.online/">
                  <i class="bx bx-power-off me-2"></i>
                  <span class="align-middle">Log Out</span>
                </a>
              </li>
            </ul>
          </li>
          <!--/ User -->
          

        </ul>
      </div>

      
      
  </nav>
  

  
<!-- / Navbar -->

      

      <!-- Content wrapper -->
      <div class="content-wrapper">

        <!-- Content -->
        
          <div class="container-xxl flex-grow-1 container-p-y">
            
            

<div class="row">
  <div class="col-lg-8 mb-4 order-0">
    <div class="card">
      <div class="d-flex align-items-end row">
        <div class="col-sm-7">
          <div class="card-body">
            <h5 class="card-title text-primary">Welcome, <?php echo $_SESSION['usuario'];?>! 🎉</h5>
            <p class="mb-4">Here you will found download of the Loader 🚀</p>

          </div>
        </div>
        <div class="col-sm-5 text-center text-sm-left">
          
            <div class="img-wrap">
              <img src="photos/<?php echo $_SESSION['usuario'];?>.png" id="img1" width="340" height="160" alt=""/>
          </div>
        </div>
      </div>
    </div>
  </div>
  <div class="col-lg-8 mb-4 order-0">
    <div class="card">
      <div class="d-flex align-items-end row">
        <div class="col-sm-7">
          <div class="card-body">
            <h5 class="card-title text-primary">To change your photo, please, paste a link of your image!</h5>
            
 <form action="" method="post" enctype="multipart/form-data">
								<div class="mb-3">
                                    <label for="username" class="form-label">Image</label>
                                    <input class="form-control" name="txt_imageuser" type="username" id="username" required="" placeholder="Enter your image link">
                                </div>
      <div class="flex-btn">
         <input class="btn btn-primary" type="submit" value="Update" name="update" class="btn">
      </div>
   </form>

          </div>
        </div>
        <div class="col-sm-5 text-center text-sm-left">

        </div>
      </div>
    </div>
  </div>

<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>
<h1></h1>

            
          </div>
          <!-- / Content -->

          
          

<!-- Footer -->
<footer class="content-footer footer bg-footer-theme">
  <div class="container-xxl d-flex flex-wrap justify-content-between py-2 flex-md-row flex-column">
    <div class="mb-2 mb-md-0">
     Copyright © <script>
      document.write(new Date().getFullYear())
      </script>, made with ❤️ by LA
    </div>
    <div>
      
    
    </div>
  </div>
</footer>
<!-- / Footer -->

          
          <div class="content-backdrop fade"></div>
        </div>
        <!-- Content wrapper -->
      </div>
      <!-- / Layout page -->
    </div>

    
    
    <!-- Overlay -->
    <div class="layout-overlay layout-menu-toggle"></div>
    
    
  </div>
  <!-- / Layout wrapper -->

  


  <!-- Core JS -->
  <!-- build:js assets/vendor/../js/core.js -->
  <script src="../js/jquery.js"></script>
  <script src="../js/popper.js"></script>
  <script src="../js/bootstrap.js"></script>
  <script src="../js/perfect-scrollbar.js"></script>
  
  <script src="../js/menu.js"></script>
  <!-- endbuild -->

  <!-- Vendors JS -->
  <script src="../js/apexcharts.js"></script>

  <!-- Main JS -->
  <script src="../js/main.js"></script>

  <!-- Page JS -->
  <script src="../js/dashboards-analytics.js"></script>
  
  <!-- Place this tag in your head or just before your close body tag. -->
  <script async defer src="../js/buttons.js"></script>
  
</body>

</html>
