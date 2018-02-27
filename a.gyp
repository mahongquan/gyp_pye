{
    "targets": [
        {
            "target_name": "myapp",
            "type": "executable",
            "sources": [
                "./9111FILE.c"
            ], "include_dirs": [
                "../../../include"
            ],
           'link_settings': {

                'libraries': [

                '../../../lib/pci-dask.lib',

                ],

            },

        }
    ]
}