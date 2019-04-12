package com.gavinandre.androidjnidemo.base

import android.app.Application
import kotlin.properties.ReadWriteProperty
import kotlin.reflect.KProperty

class MyApplication : Application() {
    
    companion object {
        
        private val TAG = MyApplication::class.java.simpleName
        
        var application: Application by DelegatesExt.notNullSingleValue()
    }
    
    override fun onCreate() {
        super.onCreate()
        application = this
    }
    
    /**
     * 创建单例使用notnull委托
     */
    object DelegatesExt {
        fun <T> notNullSingleValue():
                ReadWriteProperty<Any?, T> = NotNullSingleValueVar()
    }
    
    /**
     * 创建一个notNull的委托，只能赋值一次，多次赋值抛异常
     */
    private class NotNullSingleValueVar<T> : ReadWriteProperty<Any?, T> {
        private var value: T? = null
        override fun getValue(thisRef: Any?, property: KProperty<*>): T {
            return value ?: throw IllegalStateException("MyApplication application not initialized")
        }
        
        override fun setValue(thisRef: Any?, property: KProperty<*>, value: T) {
            this.value = if (this.value == null) value
            else throw IllegalStateException("MyApplication application already initialized")
        }
    }
}
