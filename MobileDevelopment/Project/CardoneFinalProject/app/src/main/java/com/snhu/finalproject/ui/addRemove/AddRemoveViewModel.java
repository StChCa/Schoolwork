package com.snhu.finalproject.ui.addRemove;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class AddRemoveViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public AddRemoveViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is addRemove fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}