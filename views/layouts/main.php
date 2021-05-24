<!DOCTYPE html>
<html lang="ko">
<head>
	{layout_head}
</head>
<body id="page-top">

	<!-- Page Wrapper -->
	<div id="wrapper">
		
		<!-- Sidebar -->
		{layout_sidebar}
		<!-- End of Sidebar -->
		
		<!-- Content Wrapper -->
		<div id="content-wrapper" class="d-flex flex-column">

			<!-- Main Content -->
			<div id="content">
				
				<!-- Topbar -->
				{layout_topbar}
				<!-- End of Topbar -->

				<!-- Begin Page Content -->
				<div class="container-fluid">
					
					<!-- Page Heading -->
					<div class="d-sm-flex align-items-center justify-content-between mb-4">
						<h1 class="h3 mb-0 text-gray-800">Dashboard</h1>
						<a href="#" class="d-none d-sm-inline-block btn btn-sm btn-primary shadow-sm"><i
								class="fas fa-download fa-sm text-white-50"></i> Generate Report</a>
					</div>

					<!-- layout_content -->
					{layout_content}
					<!-- End layout_content -->
					
				</div>
				<!-- /.container-fluid -->
				
				
			</div>
			<!-- End of Main Content -->

			<!-- footer -->
			{layout_footer}

		</div>
		<!-- End of Content Wrapper -->
		
	</div>
	<!-- End of Page Wrapper -->

	<!-- Scroll to Top Button-->
	<a class="scroll-to-top rounded" href="#page-top">
		<i class="fas fa-angle-up"></i>
	</a>

	<!-- Logout Modal-->
	{layout_logout}
	
	{layout_javascript}
</body>
</html>

