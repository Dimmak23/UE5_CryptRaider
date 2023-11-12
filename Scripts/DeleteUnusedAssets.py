#! This script was tested: it worked

import unreal

workingPath = "/Game/"

@unreal.uclass()
class GetEditorAssetLibrary(unreal.EditorAssetLibrary):
    pass

editorAssetLib = GetEditorAssetLibrary();

allAssets = editorAssetLib.list_assets(workingPath, True, False)

processingAssetPath = ""
allAssetsCount = len(allAssets)
if ( allAssetsCount > 0):
    with unreal.ScopedSlowTask(allAssetsCount, processingAssetPath) as slowTask:
        slowTask.make_dialog(True)
        for asset in allAssets:
            processingAssetPath = asset
            deps = editorAssetLib.find_package_referencers_for_asset(asset, False)
            if (len(deps) <= 0):
            # if (len(deps) == 0): # Consider to chage this line to comparing with '0', no need to check negative lenght
                print (">>> Deleting >>> %s" % asset)
                editorAssetLib.delete_asset(asset)
            if slowTask.should_cancel():
                break
            slowTask.enter_progress_frame(1, processingAssetPath)
