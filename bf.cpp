for(int I = 0; I < testcase; i++){
	int left, right = 0;
	cin >> left >> right;
	int max = arr[left-1];
	for(int j = left; j <= right-1; j++){
		if(max < arr[j])
			max = arr[j];
	}
	cout << max << ‘\n’;
}   
