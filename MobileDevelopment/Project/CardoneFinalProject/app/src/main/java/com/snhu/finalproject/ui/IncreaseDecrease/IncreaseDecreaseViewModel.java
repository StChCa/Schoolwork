package com.snhu.finalproject.ui.IncreaseDecrease;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class IncreaseDecreaseViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public IncreaseDecreaseViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is Increase/decrease fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}